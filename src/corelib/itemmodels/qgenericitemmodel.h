// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGENERICITEMMODEL_H
#define QGENERICITEMMODEL_H

#include <QtCore/qgenericitemmodel_impl.h>
#include <QtCore/qmap.h>

QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QGenericItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    template <typename Range,
              QGenericItemModelDetails::if_is_range<Range> = true>
    explicit QGenericItemModel(Range &&range, QObject *parent = nullptr);

    ~QGenericItemModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &data, int role = Qt::EditRole) override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &data) override;
    bool clearItemData(const QModelIndex &index) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = {}) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = {}) override;
    bool insertRows(int row, int count, const QModelIndex &parent = {}) override;
    bool removeRows(int row, int count, const QModelIndex &parent = {}) override;

private:
    Q_DISABLE_COPY_MOVE(QGenericItemModel)

    friend class QGenericItemModelImplBase;
    struct Deleter { void operator()(QGenericItemModelImplBase *that) { that->destroy(); } };
    std::unique_ptr<QGenericItemModelImplBase, Deleter> impl;
};

// implementation of forwarders
QModelIndex QGenericItemModelImplBase::createIndex(int row, int column, const void *ptr) const
{
    return m_itemModel->createIndex(row, column, ptr);
}
QHash<int, QByteArray> QGenericItemModelImplBase::roleNames() const
{
    return m_itemModel->roleNames();
}
void QGenericItemModelImplBase::dataChanged(const QModelIndex &from, const QModelIndex &to,
                                            const QList<int> &roles)
{
    m_itemModel->dataChanged(from, to, roles);
}
void QGenericItemModelImplBase::beginInsertColumns(const QModelIndex &parent, int start, int count)
{
    m_itemModel->beginInsertColumns(parent, start, count);
}
void QGenericItemModelImplBase::endInsertColumns()
{
    m_itemModel->endInsertColumns();
}
void QGenericItemModelImplBase::beginRemoveColumns(const QModelIndex &parent, int start, int count)
{
    m_itemModel->beginRemoveColumns(parent, start, count);
}
void QGenericItemModelImplBase::endRemoveColumns()
{
    m_itemModel->endRemoveColumns();
}
void QGenericItemModelImplBase::beginInsertRows(const QModelIndex &parent, int start, int count)
{
    m_itemModel->beginInsertRows(parent, start, count);
}
void QGenericItemModelImplBase::endInsertRows()
{
    m_itemModel->endInsertRows();
}
void QGenericItemModelImplBase::beginRemoveRows(const QModelIndex &parent, int start, int count)
{
    m_itemModel->beginRemoveRows(parent, start, count);
}
void QGenericItemModelImplBase::endRemoveRows()
{
    m_itemModel->endRemoveRows();
}

template <typename Structure, typename Range>
class QGenericItemModelImpl : public QGenericItemModelImplBase
{
    Q_DISABLE_COPY_MOVE(QGenericItemModelImpl)
public:
    using range_type = std::remove_pointer_t<std::remove_reference_t<Range>>;
    using row_reference = decltype(*std::begin(std::declval<range_type&>()));
    using const_row_reference = decltype(*std::cbegin(std::declval<range_type&>()));
    using row_type = std::remove_reference_t<row_reference>;

protected:
    using Self = QGenericItemModelImpl<Structure, Range>;
    Structure& that() { return static_cast<Structure &>(*this); }
    const Structure& that() const { return static_cast<const Structure &>(*this); }

    template <typename C>
    static constexpr auto size(const C &c)
    {
        if constexpr (QGenericItemModelDetails::test_size<C>())
            return std::size(c);
        else
#if defined(__cpp_lib_ranges)
            return std::ranges::distance(std::begin(c), std::end(c));
#else
            return std::distance(std::begin(c), std::end(c));
#endif
    }

    friend class tst_QGenericItemModel;
    using range_features = QGenericItemModelDetails::range_traits<range_type>;
    using row_features = QGenericItemModelDetails::range_traits<row_type>;

    using row_traits = QGenericItemModelDetails::row_traits<q20::remove_cvref_t<
                                                            std::remove_pointer_t<row_type>>>;

    static constexpr bool isMutable()
    {
        return range_features::is_mutable && row_features::is_mutable
            && std::is_reference_v<row_reference>;
    }

    static constexpr int static_row_count = QGenericItemModelDetails::static_size_v<range_type>;
    static constexpr bool rows_are_pointers = std::is_pointer_v<row_type>;
    static constexpr int static_column_count = QGenericItemModelDetails::static_size_v<row_type>;

    static constexpr bool dynamicRows() { return isMutable() && static_row_count < 0; }
    static constexpr bool dynamicColumns() { return static_column_count < 0; }

    // A row might be a value (or range of values), or a pointer.
    // row_ptr is always a pointer, and const_row_ptr is a pointer to const.
    using row_ptr = std::conditional_t<rows_are_pointers, row_type, row_type *>;
    using const_row_ptr = const std::remove_pointer_t<row_type> *;

    using ModelData = QGenericItemModelDetails::ModelData<std::conditional_t<
                                                    std::is_pointer_v<Range>,
                                                    Range, std::remove_reference_t<Range>>
                                                >;
public:
    explicit QGenericItemModelImpl(Range &&model, QGenericItemModel *itemModel)
        : QGenericItemModelImplBase(itemModel, static_cast<const Self*>(nullptr))
        , m_data{std::forward<Range>(model)}
    {
    }

    // static interface, called by QGenericItemModelImplBase
    static void callConst(ConstOp op, const QGenericItemModelImplBase *that, void *r, const void *args)
    {
        switch (op) {
        case Index: makeCall(that, &Self::index, r, args);
            break;
        case Parent: makeCall(that, &Structure::parent, r, args);
            break;
        case RowCount: makeCall(that, &Structure::rowCount, r, args);
            break;
        case ColumnCount: makeCall(that, &Structure::columnCount, r, args);
            break;
        case Flags: makeCall(that, &Self::flags, r, args);
            break;
        case HeaderData: makeCall(that, &Self::headerData, r, args);
            break;
        case Data: makeCall(that, &Self::data, r, args);
            break;
        case ItemData: makeCall(that, &Self::itemData, r, args);
            break;
        }
    }

    static void call(Op op, QGenericItemModelImplBase *that, void *r, const void *args)
    {
        switch (op) {
        case Destroy: delete static_cast<Structure *>(that);
            break;
        case SetData: makeCall(that, &Self::setData, r, args);
            break;
        case SetItemData: makeCall(that, &Self::setItemData, r, args);
            break;
        case ClearItemData: makeCall(that, &Self::clearItemData, r, args);
            break;
        case InsertColumns: makeCall(that, &Self::insertColumns, r, args);
            break;
        case RemoveColumns: makeCall(that, &Self::removeColumns, r, args);
            break;
        case InsertRows: makeCall(that, &Self::insertRows, r, args);
            break;
        case RemoveRows: makeCall(that, &Self::removeRows, r, args);
            break;
        }
    }

    // actual implementations
    QModelIndex index(int row, int column, const QModelIndex &parent) const
    {
        if (row < 0 || column < 0 || column >= that().columnCount(parent)
                                  || row >= that().rowCount(parent)) {
            return {};
        }

        return that().indexImpl(row, column, parent);
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        Qt::ItemFlags f = Structure::defaultFlags();

        if constexpr (static_column_count <= 0) {
            if constexpr (isMutable())
                f |= Qt::ItemIsEditable;
        } else if constexpr (std::is_reference_v<row_reference> && !std::is_const_v<row_reference>) {
            // we want to know if the elements in the tuple are const; they'd always be, if
            // we didn't remove the const of the range first.
            const_row_reference row = rowData(index);
            row_reference mutableRow = const_cast<row_reference>(row);
            for_element_at(mutableRow, index.column(), [&f](auto &&ref){
                using target_type = decltype(ref);
                if constexpr (std::is_const_v<std::remove_reference_t<target_type>>)
                    f &= ~Qt::ItemIsEditable;
                else if constexpr (std::is_lvalue_reference_v<target_type>)
                    f |= Qt::ItemIsEditable;
            });
        }
        return f;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        QVariant result;
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal
         || section < 0 || section >= that().columnCount({})) {
            return m_itemModel->QAbstractItemModel::headerData(section, orientation, role);
        }

        if constexpr (static_column_count >= 1) {
            const QMetaType metaType = meta_type_at<row_type>(section);
            if (metaType.isValid())
                result = QString::fromUtf8(metaType.name());
        }
        if (!result.isValid())
            result = m_itemModel->QAbstractItemModel::headerData(section, orientation, role);
        return result;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        QVariant result;
        const auto readData = [this, &result, role](const auto &value) {
            Q_UNUSED(this);
            using value_type = q20::remove_cvref_t<decltype(value)>;
            using multi_role = QGenericItemModelDetails::is_multi_role<value_type>;
            if constexpr (multi_role::value) {
                const auto it = [this, &value, role]{
                    Q_UNUSED(this);
                    if constexpr (multi_role::int_key)
                        return std::as_const(value).find(Qt::ItemDataRole(role));
                    else
                        return std::as_const(value).find(roleNames().value(role));
                }();
                if (it != value.cend()) {
                    result = QGenericItemModelDetails::value(it);
                }
            } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
                result = read(value);
            }
        };

        if (index.isValid()) {
            const_row_reference row = rowData(index);
            if constexpr (dynamicColumns())
                readData(*std::next(std::cbegin(row), index.column()));
            else if constexpr (static_column_count == 0)
                readData(row);
            else if (QGenericItemModelDetails::isValid(row))
                for_element_at(row, index.column(), readData);
        }
        return result;
    }

    QMap<int, QVariant> itemData(const QModelIndex &index) const
    {
        QMap<int, QVariant> result;
        bool tried = false;
        const auto readItemData = [this, &result, &tried](auto &&value){
            Q_UNUSED(this);
            using value_type = q20::remove_cvref_t<decltype(value)>;
            using multi_role = QGenericItemModelDetails::is_multi_role<value_type>;
            if constexpr (multi_role()) {
                tried = true;
                if constexpr (std::is_convertible_v<value_type, decltype(result)>) {
                    result = value;
                } else {
                    for (auto it = std::cbegin(value); it != std::cend(value); ++it) {
                        int role = [this, key = QGenericItemModelDetails::key(it)]() {
                            Q_UNUSED(this);
                            if constexpr (multi_role::int_key)
                                return int(key);
                            else
                                return roleNames().key(key.toUtf8(), -1);
                        }();

                        if (role != -1)
                            result.insert(role, QGenericItemModelDetails::value(it));
                    }
                }
            }
        };

        if (index.isValid()) {
            const_row_reference row = rowData(index);
            if constexpr (dynamicColumns())
                readItemData(*std::next(std::cbegin(row), index.column()));
            else if constexpr (static_column_count == 0)
                readItemData(row);
            else if (QGenericItemModelDetails::isValid(row))
                for_element_at(row, index.column(), readItemData);

            if (!tried) // no multi-role item found
                return m_itemModel->QAbstractItemModel::itemData(index);
        }
        return result;
    }

    bool setData(const QModelIndex &index, const QVariant &data, int role)
    {
        if (!index.isValid())
            return false;

        bool success = false;
        if constexpr (isMutable()) {
            auto emitDataChanged = qScopeGuard([&success, this, &index, &role]{
                if (success) {
                    Q_EMIT dataChanged(index, index, role == Qt::EditRole
                                                     ? QList<int>{} : QList{role});
                }
            });

            const auto writeData = [this, &data, role](auto &&target) -> bool {
                using value_type = q20::remove_cvref_t<decltype(target)>;
                using multi_role = QGenericItemModelDetails::is_multi_role<value_type>;
                if constexpr (multi_role::value) {
                    Qt::ItemDataRole roleToSet = Qt::ItemDataRole(role);
                    // If there is an entry for EditRole, overwrite that; otherwise,
                    // set the entry for DisplayRole.
                    if (role == Qt::EditRole) {
                        if constexpr (multi_role::int_key) {
                            if (target.find(roleToSet) == target.end())
                                roleToSet = Qt::DisplayRole;
                        } else {
                            if (target.find(roleNames().value(roleToSet)) == target.end())
                                roleToSet = Qt::DisplayRole;
                        }
                    }
                    if constexpr (multi_role::int_key)
                        return write(target[roleToSet], data);
                    else
                        return write(target[roleNames().value(roleToSet)], data);
                } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
                    return write(target, data);
                }
                return false;
            };

            row_reference row = rowData(index);
            if constexpr (dynamicColumns()) {
                success = writeData(*std::next(std::begin(row), index.column()));
            } else if constexpr (static_column_count == 0) {
                success = writeData(row);
            } else if (QGenericItemModelDetails::isValid(row)) {
                for_element_at(row, index.column(), [&writeData, &success](auto &&target){
                    using target_type = decltype(target);
                    // we can only assign to an lvalue reference
                    if constexpr (std::is_lvalue_reference_v<target_type>
                              && !std::is_const_v<std::remove_reference_t<target_type>>) {
                        success = writeData(std::forward<target_type>(target));
                    }
                });
            }
        }
        return success;
    }

    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &data)
    {
        if (!index.isValid() || data.isEmpty())
            return false;

        bool success = false;
        if constexpr (isMutable()) {
            auto emitDataChanged = qScopeGuard([&success, this, &index, &data]{
                if (success)
                    Q_EMIT dataChanged(index, index, data.keys());
            });

            bool tried = false;
            auto writeItemData = [this, &tried, &data](auto &target) -> bool {
                Q_UNUSED(this);
                using value_type = q20::remove_cvref_t<decltype(target)>;
                using multi_role = QGenericItemModelDetails::is_multi_role<value_type>;
                if constexpr (multi_role()) {
                    using key_type = typename value_type::key_type;
                    tried = true;
                    const auto roleName = [map = roleNames()](int role) { return map.value(role); };

                    // transactional: only update target if all values from data
                    // can be stored. Storing never fails with int-keys.
                    if constexpr (!multi_role::int_key)
                    {
                        auto invalid = std::find_if(data.keyBegin(), data.keyEnd(),
                            [&roleName](int role) { return roleName(role).isEmpty(); }
                        );

                        if (invalid != data.keyEnd()) {
                            qWarning("No role name set for %d", *invalid);
                            return false;
                        }
                    }

                    for (auto &&[role, value] : data.asKeyValueRange()) {
                        if constexpr (multi_role::int_key)
                            target[static_cast<key_type>(role)] = value;
                        else
                            target[QString::fromUtf8(roleName(role))] = value;
                    }
                    return true;
                }
                return false;
            };

            row_reference row = rowData(index);
            if constexpr (dynamicColumns()) {
                success = writeItemData(*std::next(std::begin(row), index.column()));
            } else if constexpr (static_column_count == 0) {
                success = writeItemData(row);
            } else if (QGenericItemModelDetails::isValid(row)) {
                for_element_at(row, index.column(), [&writeItemData, &success](auto &&target){
                    using target_type = decltype(target);
                    // we can only assign to an lvalue reference
                    if constexpr (std::is_lvalue_reference_v<target_type>
                              && !std::is_const_v<std::remove_reference_t<target_type>>) {
                        success = writeItemData(std::forward<target_type>(target));
                    }
                });
            }

            if (!tried) {
                // setItemData will emit the dataChanged signal
                emitDataChanged.dismiss();
                success = m_itemModel->QAbstractItemModel::setItemData(index, data);
            }
        }
        return success;
    }

    bool clearItemData(const QModelIndex &index)
    {
        if (!index.isValid())
            return false;

        bool success = false;
        if constexpr (isMutable()) {
            auto emitDataChanged = qScopeGuard([&success, this, &index]{
                if (success)
                    Q_EMIT dataChanged(index, index, {});
            });

            row_reference row = rowData(index);
            if constexpr (dynamicColumns()) {
                *std::next(std::begin(row), index.column()) = {};
                success = true;
            } else if constexpr (static_column_count == 0) {
                row = row_type{};
                success = true;
            } else if (QGenericItemModelDetails::isValid(row)) {
                for_element_at(row, index.column(), [&success](auto &&target){
                    using target_type = decltype(target);
                    if constexpr (std::is_lvalue_reference_v<target_type>
                               && !std::is_const_v<std::remove_reference_t<target_type>>) {
                        target = {};
                        success = true;
                    }
                });
            }
        }
        return success;
    }

    bool insertColumns(int column, int count, const QModelIndex &parent)
    {
        if constexpr (dynamicColumns() && isMutable() && row_features::has_insert) {
            if (count == 0)
                return false;
            range_type * const children = childRange(parent);
            if (!children)
                return false;

            beginInsertColumns(parent, column, column + count - 1);
            for (auto &child : *children)
                child.insert(std::next(std::begin(child), column), count, {});
            endInsertColumns();
            return true;
        }
        return false;
    }

    bool removeColumns(int column, int count, const QModelIndex &parent)
    {
        if constexpr (dynamicColumns() && isMutable() && row_features::has_erase) {
            if (column < 0 || column + count > that().columnCount(parent))
                return false;

            range_type * const children = childRange(parent);
            if (!children)
                return false;

            beginRemoveColumns(parent, column, column + count - 1);
            for (auto &child : *children) {
                const auto start = std::next(std::begin(child), column);
                child.erase(start, std::next(start, count));
            }
            endRemoveColumns();
            return true;
        }
        return false;
    }

    bool insertRows(int row, int count, const QModelIndex &parent)
    {
        if constexpr (Structure::canInsertRows()) {
            // If we operate on dynamic columns and cannot resize a newly
            // constructed row, then we cannot insert.
            if constexpr (dynamicColumns() && !row_features::has_resize)
                return false;
            range_type *children = childRange(parent);
            if (!children)
                return false;

            beginInsertRows(parent, row, row + count - 1);

            const auto pos = std::next(std::begin(*children), row);
            if constexpr (rows_are_pointers) {
                auto start = children->insert(pos, count, nullptr);
                auto end = std::next(start, count);
                for (auto it = start; it != end; ++it)
                    *it = that().makeEmptyRow(parent);
            } else {
                row_type empty_value = that().makeEmptyRow(parent);
                children->insert(pos, count, empty_value);
            }

            endInsertRows();
            return true;
        } else {
            return false;
        }
    }

    bool removeRows(int row, int count, const QModelIndex &parent = {})
    {
        if constexpr (Structure::canRemoveRows()) {
            const int prevRowCount = that().rowCount(parent);
            if (row < 0 || row + count > prevRowCount)
                return false;

            range_type *children = childRange(parent);
            if (!children)
                return false;

            beginRemoveRows(parent, row, row + count - 1);
            [[maybe_unused]] bool callEndRemoveColumns = false;
            if constexpr (dynamicColumns()) {
                // if we remove the last row in a dynamic model, then we no longer
                // know how many columns we should have, so they will be reported as 0.
                if (prevRowCount == count) {
                    if (const int columns = that().columnCount(parent)) {
                        callEndRemoveColumns = true;
                        beginRemoveColumns(parent, 0, columns - 1);
                    }
                }
            }
            { // erase invalidates iterators
                const auto start = std::next(std::begin(*children), row);
                children->erase(start, std::next(start, count));
            }
            if constexpr (dynamicColumns()) {
                if (callEndRemoveColumns) {
                    Q_ASSERT(that().columnCount(parent) == 0);
                    endRemoveColumns();
                }
            }
            endRemoveRows();
            return true;
        } else {
            return false;
        }
    }

protected:
    template <typename Value>
    static QVariant read(const Value &value)
    {
        if constexpr (std::is_constructible_v<QVariant, Value>)
            return QVariant(value);
        else
            return QVariant::fromValue(value);
    }
    template <typename Value>
    static QVariant read(Value *value)
    {
        if (value) {
            if constexpr (std::is_constructible_v<QVariant, Value *>)
                return QVariant(value);
            else
                return read(*value);
        }
        return {};
    }

    template <typename Target>
    static bool write(Target &target, const QVariant &value)
    {
        using Type = std::remove_reference_t<Target>;
        if constexpr (std::is_constructible_v<Target, QVariant>) {
            target = value;
            return true;
        } else if (value.canConvert<Type>()) {
            target = value.value<Type>();
            return true;
        }
        return false;
    }
    template <typename Target>
    static bool write(Target *target, const QVariant &value)
    {
        if (target)
            return write(*target, value);
        return false;
    }

    // helpers
    const_row_reference rowData(const QModelIndex &index) const
    {
        Q_ASSERT(index.isValid());
        return that().rowDataImpl(index);
    }

    row_reference rowData(const QModelIndex &index)
    {
        Q_ASSERT(index.isValid());
        return that().rowDataImpl(index);
    }

    const range_type *childRange(const QModelIndex &index) const
    {
        if (!index.isValid())
            return m_data.model();
        if (index.column()) // only items at column 0 can have children
            return nullptr;
        return that().childRangeImpl(index);
    }

    range_type *childRange(const QModelIndex &index)
    {
        if (!index.isValid())
            return m_data.model();
        if (index.column()) // only items at column 0 can have children
            return nullptr;
        return that().childRangeImpl(index);
    }

    ModelData m_data;
};

template <typename Range, typename Protocol>
class QGenericItemModelStructureImpl : public QGenericItemModelImpl<
                                                QGenericItemModelStructureImpl<Range, Protocol>,
                                              Range>
{};

// specialization for flat models without protocol
template <typename Range>
class QGenericItemModelStructureImpl<Range, void> : public QGenericItemModelImpl<
                                                QGenericItemModelStructureImpl<Range, void>,
                                            Range>
{
    using Base = QGenericItemModelImpl<QGenericItemModelStructureImpl<Range, void>, Range>;
    friend class QGenericItemModelImpl<QGenericItemModelStructureImpl<Range, void>, Range>;

    using range_features = typename Base::range_features;
    using row_type = typename Base::row_type;
    using row_traits = typename Base::row_traits;
    using row_features = typename Base::row_features;

public:
    explicit QGenericItemModelStructureImpl(Range &&model, QGenericItemModel *itemModel)
        : Base(std::forward<Range>(model), itemModel)
    {}

protected:
    QModelIndex indexImpl(int row, int column, const QModelIndex &) const
    {
        if constexpr (Base::dynamicColumns()) {
            if (column < int(Base::size(*std::next(std::cbegin(*this->m_data.model()), row))))
                return this->createIndex(row, column);
            // if we got here, then column < columnCount(), but this row is to short
            qCritical("QGenericItemModel: Column-range at row %d is not large enough!", row);
            return {};
        } else {
            return this->createIndex(row, column);
        }
    }

    QModelIndex parent(const QModelIndex &) const
    {
        return {};
    }

    int rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;
        return int(Base::size(*this->m_data.model()));
    }

    int columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;

        // in a table, all rows have the same number of columns (as the first row)
        if constexpr (Base::dynamicColumns()) {
            return int(Base::size(*this->m_data.model()) == 0
                       ? 0
                       : Base::size(*std::cbegin(*this->m_data.model())));
        } else if constexpr (Base::static_column_count == 0) {
            return row_traits::fixed_size();
        } else {
            return Base::static_column_count;
        }
    }

    static constexpr Qt::ItemFlags defaultFlags()
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
    }

    static constexpr bool canInsertRows()
    {
        return Base::dynamicRows() && range_features::has_insert;
    }

    static constexpr bool canRemoveRows()
    {
        return Base::dynamicRows() && range_features::has_erase;
    }

    auto makeEmptyRow(const QModelIndex &)
    {
        if constexpr (Base::dynamicColumns()) {
            // all rows have to have the same column count
            row_type empty_row;
            if constexpr (row_features::has_resize)
                empty_row.resize(Base::m_itemModel->columnCount());
            return empty_row;
        } else {
            return row_type{};
        }
    }

    decltype(auto) rowDataImpl(const QModelIndex &index) const
    {
        Q_ASSERT(index.row() < int(Base::size(*this->m_data.model())));
        return *(std::next(std::cbegin(*this->m_data.model()), index.row()));
    }

    decltype(auto) rowDataImpl(const QModelIndex &index)
    {
        Q_ASSERT(index.row() < int(Base::size(*this->m_data.model())));
        return *(std::next(std::begin(*this->m_data.model()), index.row()));
    }

    auto childRangeImpl(const QModelIndex &) const
    {
        return nullptr;
    }

    auto childRangeImpl(const QModelIndex &)
    {
        return nullptr;
    }
};

template <typename Range,
          QGenericItemModelDetails::if_is_range<Range>>
QGenericItemModel::QGenericItemModel(Range &&range, QObject *parent)
    : QAbstractItemModel(parent)
    , impl(new QGenericItemModelStructureImpl<Range, void>(std::forward<Range>(range), this))
{}

QT_END_NAMESPACE

#endif // QGENERICITEMMODEL_H
