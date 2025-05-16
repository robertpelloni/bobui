// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGENERICITEMMODEL_H
#define QGENERICITEMMODEL_H

#include <QtCore/qgenericitemmodel_impl.h>

QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QGenericItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    template <typename T>
    using SingleColumn = std::tuple<T>;

    template <typename T>
    struct MultiColumn
    {
        using type = std::remove_pointer_t<T>;
        T data{};
        template <typename X>
        using if_get_matches = std::enable_if_t<std::is_same_v<q20::remove_cvref_t<X>,
                                                               MultiColumn<T>>, bool>;

        template <typename V = T,
                  std::enable_if_t<QGenericItemModelDetails::is_validatable<V>::value, bool> = true>
        constexpr explicit operator bool() const noexcept { return bool(data); }

        // unconstrained on size_t I, gcc internal error #3280
        template <std::size_t I, typename V, if_get_matches<V> = true>
        friend inline decltype(auto) get(V &&multiColumn)
        {
            static_assert(I < std::tuple_size_v<type>, "Index out of bounds for wrapped type");
            return get<I>(QGenericItemModelDetails::refTo(q23::forward_like<V>(multiColumn.data)));
        }
    };

    template <typename Range,
              QGenericItemModelDetails::if_is_table_range<Range> = true>
    explicit QGenericItemModel(Range &&range, QObject *parent = nullptr);

    template <typename Range,
              QGenericItemModelDetails::if_is_tree_range<Range> = true>
    explicit QGenericItemModel(Range &&range, QObject *parent = nullptr);

    template <typename Range, typename Protocol,
              QGenericItemModelDetails::if_is_tree_range<Range, Protocol> = true>
    explicit QGenericItemModel(Range &&range, Protocol &&protocol, QObject *parent = nullptr);

    ~QGenericItemModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &index) const override;
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
    bool moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count,
                     const QModelIndex &destParent, int destColumn) override;
    bool insertRows(int row, int count, const QModelIndex &parent = {}) override;
    bool removeRows(int row, int count, const QModelIndex &parent = {}) override;
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                  const QModelIndex &destParent, int destRow) override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex buddy(const QModelIndex &index) const override;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                         const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                      const QModelIndex &parent) override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    QStringList mimeTypes() const override;
    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits,
                          Qt::MatchFlags flags) const override;
    void multiData(const QModelIndex &index, QModelRoleDataSpan roleDataSpan) const override;
    QHash<int, QByteArray> roleNames() const override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    QSize span(const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

protected Q_SLOTS:
    void resetInternalData() override;

protected:
    bool event(QEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;

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
void QGenericItemModelImplBase::changePersistentIndexList(const QModelIndexList &from,
                                                          const QModelIndexList &to)
{
    m_itemModel->changePersistentIndexList(from, to);
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
bool QGenericItemModelImplBase::beginMoveColumns(const QModelIndex &sourceParent, int sourceFirst,
                                                 int sourceLast, const QModelIndex &destParent,
                                                 int destColumn)
{
    return m_itemModel->beginMoveColumns(sourceParent, sourceFirst, sourceLast,
                                         destParent, destColumn);
}
void QGenericItemModelImplBase::endMoveColumns()
{
    m_itemModel->endMoveColumns();
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
bool QGenericItemModelImplBase::beginMoveRows(const QModelIndex &sourceParent, int sourceFirst,
                                              int sourceLast,
                                              const QModelIndex &destParent, int destRow)
{
    return m_itemModel->beginMoveRows(sourceParent, sourceFirst, sourceLast, destParent, destRow);
}
void QGenericItemModelImplBase::endMoveRows()
{
    m_itemModel->endMoveRows();
}
QAbstractItemModel &QGenericItemModelImplBase::itemModel()
{
    return *m_itemModel;
}
const QAbstractItemModel &QGenericItemModelImplBase::itemModel() const
{
    return *m_itemModel;
}

template <typename Range,
          QGenericItemModelDetails::if_is_table_range<Range>>
QGenericItemModel::QGenericItemModel(Range &&range, QObject *parent)
    : QAbstractItemModel(parent)
    , impl(new QGenericTableItemModelImpl<Range>(std::forward<Range>(range), this))
{}

template <typename Range,
         QGenericItemModelDetails::if_is_tree_range<Range>>
QGenericItemModel::QGenericItemModel(Range &&range, QObject *parent)
    : QGenericItemModel(std::forward<Range>(range),
                        QGenericItemModelDetails::DefaultTreeProtocol<Range>{}, parent)
{}

template <typename Range, typename Protocol,
          QGenericItemModelDetails::if_is_tree_range<Range, Protocol>>
QGenericItemModel::QGenericItemModel(Range &&range, Protocol &&protocol, QObject *parent)
    : QAbstractItemModel(parent)
   , impl(new QGenericTreeItemModelImpl<Range, Protocol>(std::forward<Range>(range),
                                                         std::forward<Protocol>(protocol), this))
{}

QT_END_NAMESPACE

namespace std {
    template <typename T>
    struct tuple_size<QT_PREPEND_NAMESPACE(QGenericItemModel)::MultiColumn<T>>
        : tuple_size<typename QT_PREPEND_NAMESPACE(QGenericItemModel)::MultiColumn<T>::type>
    {};
    template <std::size_t I, typename T>
    struct tuple_element<I, QT_PREPEND_NAMESPACE(QGenericItemModel)::MultiColumn<T>>
        : tuple_element<I, typename QT_PREPEND_NAMESPACE(QGenericItemModel)::MultiColumn<T>::type>
    {};
}

#endif // QGENERICITEMMODEL_H
