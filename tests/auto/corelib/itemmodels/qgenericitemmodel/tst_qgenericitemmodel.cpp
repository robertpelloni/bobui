// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QTest>
#include <QtCore/qgenericitemmodel.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonarray.h>

#include <QtGui/qcolor.h>

#if QT_CONFIG(itemmodeltester)
#include <QtTest/qabstractitemmodeltester.h>
#endif

#include <list>
#include <vector>

#if defined(__cpp_lib_ranges)
#include <ranges>
#endif

class Item
{
    Q_GADGET
    Q_PROPERTY(QString display READ display WRITE setDisplay)
    Q_PROPERTY(QColor decoration READ decoration WRITE setDecoration)
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
public:
    Item() = default;

    Item(const QString &display, QColor decoration, const QString &toolTip)
        : m_display(display), m_decoration(decoration), m_toolTip(toolTip)
    {
    }

    QString display() const { return m_display; }
    void setDisplay(const QString &display) { m_display = display; }
    QColor decoration() const { return m_decoration; }
    void setDecoration(QColor decoration) { m_decoration = decoration; }
    QString toolTip() const { return m_toolTip.isEmpty() ? display() : m_toolTip; }
    void setToolTip(const QString &toolTip) { m_toolTip = toolTip; }

private:
    QString m_display;
    QColor m_decoration;
    QString m_toolTip;
};

class Object : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString string READ string WRITE setString)
    Q_PROPERTY(int number READ number WRITE setNumber)
public:
    using QObject::QObject;

    QString string() const { return m_string; }
    void setString(const QString &string) { m_string = string; }
    int number() const { return m_number; }
    void setNumber(int number) { m_number = number; }

private:
    // note: default values need to be convertible to each other
    QString m_string = "1234";
    int m_number = 42;
};

// a class that can be both and requires disambiguation
class MetaObjectTuple : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString display MEMBER m_string)
    Q_PROPERTY(int number MEMBER m_number)
public:
    using QObject::QObject;

private:
    QString m_string = "4321";
    int m_number = 24;

    template <size_t I, typename G,
        std::enable_if_t<(I < 2), bool> = true,
        std::enable_if_t<std::is_same_v<q20::remove_cvref_t<G>, MetaObjectTuple>, bool> = true
    >
    friend inline decltype(auto) get(G &&item)
    {
        if constexpr (I == 0)
            return q23::forward_like<G>(item.m_string);
        else if constexpr (I == 1)
            return q23::forward_like<G>(item.m_number);
    }
};

namespace std {
    template <> struct tuple_size<MetaObjectTuple> : std::integral_constant<size_t, 2> {};
    template <> struct tuple_element<0, MetaObjectTuple> { using type = QString; };
    template <> struct tuple_element<1, MetaObjectTuple> { using type = int; };
}

struct Row
{
    Item m_item;
    int m_number;
    QString m_description;

    template <size_t I, typename RowType,
        std::enable_if_t<(I < 3), bool> = true,
        std::enable_if_t<std::is_same_v<q20::remove_cvref_t<RowType>, Row>, bool> = true
    >
    friend inline decltype(auto) get(RowType &&item)
    {
        if constexpr (I == 0)
            return q23::forward_like<RowType>(item.m_item);
        else if constexpr (I == 1)
            return q23::forward_like<RowType>(item.m_number);
        else // if constexpr (I == 2)
            return q23::forward_like<RowType>(item.m_description);
    }
};

namespace std {
    template <> struct tuple_size<Row> : std::integral_constant<size_t, 3> {};
    template <> struct tuple_element<0, Row> { using type = Item; };
    template <> struct tuple_element<1, Row> { using type = int; };
    template <> struct tuple_element<2, Row> { using type = QString; };
}

struct ConstRow
{
    QString value;

    template<size_t I,
        std::enable_if_t<I == 0, bool> = true
    >
    friend inline decltype(auto) get(const ConstRow &row)
    {
        if constexpr (I == 0)
            return row.value;
    }
};

namespace std {
    template <> struct tuple_size<ConstRow> : std::integral_constant<size_t, 1> {};
    template <> struct tuple_element<0, ConstRow> { using type = QString; };
}

struct tree_row;
using value_tree = QList<tree_row>;
using pointer_tree = QList<tree_row *>;

struct tree_row
{
public:
    tree_row(const QString &value = {}, const QString &description = {})
        : m_value(value), m_description(description)
    {}

    ~tree_row()
    {
        if (m_childrenPointers)
            qDeleteAll(*m_childrenPointers);
    }

    tree_row(const tree_row &other)
        : m_value(other.m_value), m_description(other.m_description)
        , m_parent(other.m_parent), m_children(other.m_children)
        , m_childrenPointers(other.m_childrenPointers)
    {}

    tree_row &operator=(const tree_row &other)
    {
        m_parent = other.m_parent;
        m_children = other.m_children;
        m_childrenPointers = other.m_childrenPointers;
        m_value = other.m_value;
        m_description = other.m_description;
        return *this;
    }

    tree_row(tree_row &&other) = default;
    tree_row &operator=(tree_row &&other) = default;

    QString &value() { return m_value; }
    const QString &value() const { return m_value; }
    QString &description() { return m_description; }
    const QString &description() const { return m_description; }

    template <typename ...Args>
    tree_row &addChild(Args&& ...args)
    {
        if (!m_children)
            m_children.emplace(value_tree{});
        tree_row &res = m_children->emplace_back(args...);
        res.m_parent = this;
        return res;
    }

    template <typename ...Args>
    tree_row *addChildPointer(Args&& ...args)
    {
        if (!m_childrenPointers)
            m_childrenPointers.emplace(pointer_tree{});
        auto *res = new tree_row(args...);
        m_childrenPointers->push_back(res);
        res->m_parent = this;
        return res;
    }

    const tree_row *parentRow() const { return m_parent; }
    void setParentRow(tree_row *parent) { m_parent = parent; }
    const std::optional<value_tree> &childRows() const { return m_children; }
    std::optional<value_tree> &childRows() { return m_children; }

    static void prettyPrint(QDebug dbg, const value_tree &tree, int depth = 0)
    {
        dbg.nospace().noquote();
        const QString indent(depth * 2, ' ');
        bool first = true;
        for (const auto &row : tree) {
            dbg << indent;
            if (first && depth) {
                dbg << "\\";
                first = false;
            } else {
                dbg << "|";
            }
            dbg << row << "\n";
            if (const auto &children = row.childRows())
                prettyPrint(dbg, *children, depth + 1);
        }
    }

    struct ProtocolPointerImpl {
        tree_row *newRow() const { return new tree_row; }
        void deleteRow(tree_row *row) { delete row; }
        const tree_row *parentRow(const tree_row *row) const { return row->m_parent; }
        void setParentRow(tree_row *row, tree_row *parent) { row->m_parent = parent; }

        const std::optional<pointer_tree> &childRows(const tree_row *row) const
        { return row->m_childrenPointers; }
        std::optional<pointer_tree> &childRows(tree_row *row)
        { return row->m_childrenPointers; }
    };

private:
    QString m_value;
    QString m_description;

    tree_row *m_parent = nullptr;
    std::optional<value_tree> m_children = std::nullopt;
    std::optional<pointer_tree> m_childrenPointers = std::nullopt;

    friend inline QDebug operator<<(QDebug dbg, const tree_row &row)
    {
        QDebugStateSaver saver(dbg);
        dbg.nospace() << row.m_value << " : " << row.m_description;
        if (row.parentRow())
            dbg << " ^ " << row.parentRow()->value();
        if (row.childRows())
            dbg << " v " << row.childRows()->size();
        return dbg;
    }

    template<size_t I, typename Row,
        std::enable_if_t<std::is_same_v<q20::remove_cvref_t<Row>, tree_row>, bool> = true>
    friend inline decltype(auto) get(Row &&row)
    {
        if constexpr (I == 0)
            return row.value();
        else if constexpr (I == 1)
            return row.description();
    }
};

namespace std {
    template <> struct tuple_size<tree_row> : std::integral_constant<size_t, 2> {};
    template <size_t I> struct tuple_element<I, tree_row>
    { using type = decltype(get<I>(std::declval<tree_row>())); };
}

class tst_QGenericItemModel : public QObject
{
    Q_OBJECT

private slots:
    void basics_data() { createTestData(); }
    void basics();
    void minimalIterator();
    void ranges();
    void json();
    void ownership();

    void dimensions_data() { createTestData(); }
    void dimensions();
    void flags_data() { createTestData(); }
    void flags();
    void data_data() { createTestData(); }
    void data();
    void setData_data() { createTestData(); }
    void setData();
    void itemData_data() { createTestData(); }
    void itemData();
    void setItemData_data() { createTestData(); }
    void setItemData();
    void clearItemData_data() { createTestData(); }
    void clearItemData();
    void insertRows_data() { createTestData(); }
    void insertRows();
    void removeRows_data() { createTestData(); }
    void removeRows();
    void insertColumns_data() { createTestData(); }
    void insertColumns();
    void removeColumns_data() { createTestData(); }
    void removeColumns();

    void inconsistentColumnCount();

    void tree_data();
    void tree();
    void treeModifyBranch_data() { tree_data(); }
    void treeModifyBranch();
    void treeCreateBranch_data() { tree_data(); }
    void treeCreateBranch();
    void treeRemoveBranch_data() { tree_data(); }
    void treeRemoveBranch();

private:
    enum TestedModels {
        Lists   = 0x01,
        Tables  = 0x02,
        Trees   = 0x04,
        All     = Lists|Tables|Trees,
    };
    void createTestData(TestedModels tested = All);
    void createTree();

    QList<QPersistentModelIndex> allIndexes(QAbstractItemModel *model, const QModelIndex &parent = {})
    {
        QList<QPersistentModelIndex> pmiList;
        for (int row = 0; row < model->rowCount(parent); ++row) {
            const QModelIndex mi = model->index(row, 0, parent);
            pmiList += mi;
            if (model->hasChildren(mi))
                pmiList += allIndexes(model, mi);
        }
        return pmiList;
    }

    void verifyPmiList(const QList<QPersistentModelIndex> &pmiList)
    {
        for (const auto &pmi : pmiList) {
            auto debug = qScopeGuard([&pmi]{
                qCritical() << "Failing index" << pmi << pmi.isValid();
            });
            QVERIFY(pmi.isValid());
            QVERIFY(pmi.data().isValid());
            QCOMPARE(pmi.parent().isValid(), pmi.parent().data().isValid());
            debug.dismiss();
        }
    }

    template <typename Tree>
    static bool integrityCheck(const Tree &tree, int depth = 0)
    {
        static constexpr bool pointerTree = std::is_pointer_v<typename std::remove_reference_t<Tree>::value_type>;
        bool result = true;
        for (const auto &row : tree) {
            if constexpr (pointerTree) {
                if (!row) {
                    qCritical() << "Unexpected null pointer in tree!";
                    return false;
                }
            }
            const auto &children = [&row]() -> const auto &{
                if constexpr (pointerTree) {
                    const auto protocol = tree_row::ProtocolPointerImpl{};
                    return protocol.childRows(row);
                } else {
                    return row.childRows();
                }
            }();
            if (children) {
                for (const auto &child : *children) {
                    const bool match = [&child, &row]{
                        if constexpr (pointerTree) {
                            if (child->parentRow() != row) {
                                qCritical().noquote() << "Parent out of sync for:" << *child;
                                qCritical().noquote() << "  Actual: " << child->parentRow()
                                        << (child->parentRow() ? *child->parentRow() : tree_row{});
                                qCritical().noquote() << "Expected: " << row << *row;
                                return false;
                            }
                        } else {
                            if (child.parentRow() != std::addressof(row)) {
                                qCritical().noquote() << "Parent out of sync for:" << child;
                                qCritical().noquote() << "  Actual: " << child.parentRow()
                                        << (child.parentRow() ? *child.parentRow() : tree_row{});
                                qCritical().noquote() << "Expected: " << std::addressof(row) << row;
                                return false;
                            }
                        }
                        return true;
                    }();
                    if (!match)
                        return false;
                }
                result &= integrityCheck(*children, depth + 1);
            }
        }
        return result;
    }
    bool treeIntegrityCheck()
    {
        if (!integrityCheck(*m_data->m_tree)) {
            tree_row::prettyPrint(qDebug().nospace() << "\nTree of Values:\n", *m_data->m_tree);
            return false;
        }
        if (!integrityCheck(*m_data->m_pointer_tree)) {
            tree_row::prettyPrint(qDebug().nospace() << "\nTree of Pointers:\n", *m_data->m_tree);
            return false;
        }
        return true;
    }

    std::unique_ptr<QAbstractItemModel> makeTreeModel();

    struct Data {

        // fixed number of columns and rows
        std::array<int, 5> fixedArrayOfNumbers = {1, 2, 3, 4, 5};
        int cArrayOfNumbers[5] = {1, 2, 3, 4, 5};
        Row cArrayFixedColumns[3] = {
            {{"red", Qt::red, "0xff0000"}, 0xff0000, "The color red"},
            {{"green", Qt::green, "0x00ff00"}, 0x00ff00, "The color green"},
            {{"blue", Qt::blue, "0x0000ff"}, 0x0000ff, "The color blue"}
        };

        // dynamic number of rows, fixed number of columns
        std::vector<std::tuple<int, QString>> vectorOfFixedColumns = {
            {0, "null"},
            {1, "one"},
            {2, "two"},
            {3, "three"},
            {4, "four"},
        };
        std::vector<std::array<int, 10>> vectorOfArrays = {
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
            {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
            {21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
            {31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
            {41, 42, 43, 44, 45, 46, 47, 48, 49, 50},
        };
        std::vector<Item> vectorOfGadgets = {
            {"red", Qt::red, "0xff0000"},
            {"green", Qt::green, "0x00ff00"},
            {"blue", Qt::blue, "0x0000ff"},
        };
        std::vector<QGenericItemModel::SingleColumn<Item>> listOfGadgets = {
            {{"red", Qt::red, "0xff0000"}},
            {{"green", Qt::green, "0x00ff00"}},
            {{"blue", Qt::blue, "0x0000ff"}},
        };
        std::vector<Row> vectorOfStructs = {
            {{"red", Qt::red, "0xff0000"}, 1, "one"},
            {{"green", Qt::green, "0x00ff00"}, 2, "two"},
            {{"blue", Qt::blue, "0x0000ff"}, 3, "three"},
        };

        Object row1;
        Object row2;
        Object row3;
        std::list<Object *> listOfObjects = {
            &row1, &row2, &row3
        };

        MetaObjectTuple mot1;
        MetaObjectTuple mot2;
        MetaObjectTuple mot3;
        std::vector<QGenericItemModel::SingleColumn<MetaObjectTuple *>> listOfMetaObjectTuple = {
            &mot1,
            &mot2,
            &mot3,
        };
        MetaObjectTuple mot4;
        MetaObjectTuple mot5;
        MetaObjectTuple mot6;
        std::vector<QGenericItemModel::MultiColumn<MetaObjectTuple *>> tableOfMetaObjectTuple = {
            {&mot4},
            {&mot5},
            {&mot6},
        };

        // bad (but legal) get() overload that never returns a mutable reference
        std::vector<ConstRow> vectorOfConstStructs = {
            {"one"},
            {"two"},
            {"three"},
        };

        // dynamic number of rows and columns
        std::vector<std::vector<double>> tableOfNumbers = {
            {1.0, 2.0, 3.0, 4.0, 5.0},
            {6.0, 7.0, 8.0, 9.0, 10.0},
            {11.0, 12.0, 13.0, 14.0, 15.0},
            {16.0, 17.0, 18.0, 19.0, 20.0},
            {21.0, 22.0, 23.0, 24.0, 25.0},
        };

        // item is pointer
        Item itemAsPointer = {"red", Qt::red, "0xff0000"};
        std::vector<std::vector<Item *>> tableOfPointers = {
            {&itemAsPointer, &itemAsPointer},
            {&itemAsPointer, &itemAsPointer},
            {&itemAsPointer, &itemAsPointer},
        };

        // rows are pointers
        Row rowAsPointer = {{"blue", Qt::blue, "0x0000ff"}, 0x0000ff, "Blau"};
        std::vector<Row *> tableOfRowPointers = {
            &rowAsPointer,
            &rowAsPointer,
            &rowAsPointer,
        };

        // constness
        std::array<const int, 5> arrayOfConstNumbers = { 1, 2, 3, 4 };
        // note: std::vector doesn't allow for const value types
        const std::vector<int> constListOfNumbers = { 1, 2, 3 };

        // const model is read-only
        const std::vector<std::vector<double>> constTableOfNumbers = {
            {1.0, 2.0, 3.0, 4.0, 5.0},
            {6.0, 7.0, 8.0, 9.0, 10.0},
            {11.0, 12.0, 13.0, 14.0, 15.0},
            {16.0, 17.0, 18.0, 19.0, 20.0},
            {21.0, 22.0, 23.0, 24.0, 25.0},
        };

        // values are associative containers
        std::vector<QVariantMap> listOfNamedRoles = {
            {{"display", "DISPLAY0"}, {"decoration", "DECORATION0"}},
            {{"display", "DISPLAY1"}, {"decoration", "DECORATION1"}},
            {{"display", "DISPLAY2"}, {"decoration", "DECORATION2"}},
            {{"display", "DISPLAY3"}, {"decoration", "DECORATION3"}},
        };
        std::vector<std::vector<std::map<Qt::ItemDataRole, QVariant>>> tableOfEnumRoles = {
            {{{Qt::DisplayRole, "DISPLAY0"}, {Qt::DecorationRole, "DECORATION0"}}},
            {{{Qt::DisplayRole, "DISPLAY1"}, {Qt::DecorationRole, "DECORATION1"}}},
            {{{Qt::DisplayRole, "DISPLAY2"}, {Qt::DecorationRole, "DECORATION2"}}},
            {{{Qt::DisplayRole, "DISPLAY3"}, {Qt::DecorationRole, "DECORATION3"}}},
        };
        std::vector<std::vector<QMap<int, QVariant>>> tableOfIntRoles = {
            {{{Qt::DisplayRole, "DISPLAY0"}, {Qt::DecorationRole, "DECORATION0"}}},
            {{{Qt::DisplayRole, "DISPLAY1"}, {Qt::DecorationRole, "DECORATION1"}}},
            {{{Qt::DisplayRole, "DISPLAY2"}, {Qt::DecorationRole, "DECORATION2"}}},
            {{{Qt::DisplayRole, "DISPLAY3"}, {Qt::DecorationRole, "DECORATION3"}}},
        };
        std::vector<std::vector<std::map<int, QVariant>>> stdTableOfIntRoles = {
            {{{Qt::DisplayRole, "DISPLAY0"}, {Qt::DecorationRole, "DECORATION0"}}},
            {{{Qt::DisplayRole, "DISPLAY1"}, {Qt::DecorationRole, "DECORATION1"}}},
            {{{Qt::DisplayRole, "DISPLAY2"}, {Qt::DecorationRole, "DECORATION2"}}},
            {{{Qt::DisplayRole, "DISPLAY3"}, {Qt::DecorationRole, "DECORATION3"}}},
        };

        std::unique_ptr<value_tree> m_tree;
        struct TreeDeleter {
            void operator()(pointer_tree *tree)
            {
                for (auto *row : *tree)
                    delete row;
                delete tree;
            }
        };
        std::unique_ptr<pointer_tree, TreeDeleter> m_pointer_tree;
    };

    std::unique_ptr<Data> m_data;

public:
    enum class ChangeAction
    {
        ReadOnly        = 0x00,
        InsertRows      = 0x01,
        RemoveRows      = 0x02,
        ChangeRows      = InsertRows | RemoveRows,
        InsertColumns   = 0x04,
        RemoveColumns   = 0x08,
        ChangeColumns   = InsertColumns | RemoveColumns,
        SetData         = 0x10,
        All             = ChangeRows | ChangeColumns | SetData,
        SetItemData     = 0x20,
    };
    Q_DECLARE_FLAGS(ChangeActions, ChangeAction);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(tst_QGenericItemModel::ChangeActions)

using Factory = std::function<std::unique_ptr<QAbstractItemModel>()>;

// Pointer- and reference-tests will modify the data structure that lives in
// m_data, so we have to keep backup copies of that data.
template <typename T, std::enable_if_t<std::is_copy_assignable_v<T>, bool> = true>
void createBackup(QObject* object, T& model) {
    QObject::connect(object, &QObject::destroyed, object, [backup = model, &model]() mutable {
        model = backup;
    });
}

template <typename T, std::enable_if_t<!std::is_copy_assignable_v<T>, bool> = true>
void createBackup(QObject* , T& ) {}

void tst_QGenericItemModel::createTestData(TestedModels tested)
{
    m_data.reset(new Data);

    createTree();

    QTest::addColumn<Factory>("factory");
    QTest::addColumn<int>("expectedRowCount");
    QTest::addColumn<int>("expectedColumnCount");
    QTest::addColumn<ChangeActions>("changeActions");

#define ADD_HELPER(Model, Tag, Ref, ColumnCount, Actions) \
    { \
        Factory factory = [this]() -> std::unique_ptr<QAbstractItemModel> { \
            auto result = std::make_unique<QGenericItemModel>(Ref(m_data->Model)); \
            createBackup(result.get(), m_data->Model); \
            return result; \
        }; \
        QTest::addRow(#Model #Tag) << std::move(factory) << int(std::size(m_data->Model)) \
                                   << int(ColumnCount) << ChangeActions(Actions); \
    }

#define ADD_POINTER(Model, ColumnCount, Actions) ADD_HELPER(Model, Pointer, &, ColumnCount, Actions)
#define ADD_COPY(Model, ColumnCount, Actions) ADD_HELPER(Model, Copy, *&, ColumnCount, Actions)
#define ADD_REF(Model, ColumnCount, Actions) ADD_HELPER(Model, Ref, std::ref, ColumnCount, Actions)
#define ADD_ALL(Model, ColumnCount, Actions) \
    ADD_COPY(Model, ColumnCount, Actions) \
    ADD_POINTER(Model, ColumnCount, Actions) \
    ADD_REF(Model, ColumnCount, Actions)

    // The entire test data is recreated for each test function, but test
    // functions must not change data structures other than the one tested.

    ADD_ALL(fixedArrayOfNumbers, 1, ChangeAction::SetData);

    ADD_POINTER(cArrayOfNumbers, 1, ChangeAction::SetData);

    ADD_POINTER(cArrayFixedColumns,
                std::tuple_size_v<Row>,
                ChangeAction::SetData | ChangeAction::SetItemData);

    ADD_ALL(vectorOfFixedColumns, 2, ChangeAction::ChangeRows | ChangeAction::SetData);

    ADD_ALL(vectorOfArrays, 10, ChangeAction::ChangeRows | ChangeAction::SetData);

    ADD_ALL(vectorOfStructs,
            std::tuple_size_v<Row>,
            ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);

    ADD_ALL(vectorOfConstStructs, std::tuple_size_v<ConstRow>, ChangeAction::ChangeRows);

    ADD_ALL(vectorOfGadgets, 3, ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);

    ADD_ALL(listOfGadgets, 1, ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);

    ADD_COPY(listOfObjects, 2, ChangeAction::ChangeRows | ChangeAction::SetData);

    ADD_ALL(tableOfNumbers, 5, ChangeAction::All);

    // only adding as pointer, copy would operate on the same data
    ADD_POINTER(tableOfPointers, 2, ChangeAction::All | ChangeAction::SetItemData);
    ADD_POINTER(tableOfRowPointers,
                std::tuple_size_v<Row>,
                ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);

    ADD_ALL(arrayOfConstNumbers, 1, ChangeAction::ReadOnly);

    ADD_ALL(constListOfNumbers, 1, ChangeAction::ReadOnly);

    ADD_ALL(constTableOfNumbers, 5, ChangeAction::ReadOnly);

    ADD_ALL(listOfNamedRoles, 1, ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);

    ADD_ALL(tableOfEnumRoles, 1, ChangeAction::All | ChangeAction::SetItemData);

    ADD_ALL(tableOfIntRoles, 1, ChangeAction::All | ChangeAction::SetItemData);

    ADD_ALL(stdTableOfIntRoles, 1, ChangeAction::All | ChangeAction::SetItemData);

#undef ADD_COPY
#undef ADD_POINTER
#undef ADD_HELPER
#undef ADD_ALL

    QTest::addRow("Moved table") << Factory([]{
        QList<std::vector<QString>> movedTable = {
            {"0/0", "0/1", "0/2", "0/3"},
            {"1/0", "1/1", "1/2", "1/3"},
            {"2/0", "2/1", "2/2", "2/3"},
            {"3/0", "3/1", "3/2", "3/3"},
        };
        return std::unique_ptr<QAbstractItemModel>(new QGenericItemModel(std::move(movedTable)));
    }) << 4 << 4 << ChangeActions(ChangeAction::All);

    // moved list of pointers -> model takes ownership
    QTest::addRow("movedListOfObjects") << Factory([]{
        std::list<Object *> movedListOfObjects = {
            new Object, new Object, new Object,
            new Object, new Object, new Object
        };

        return std::unique_ptr<QAbstractItemModel>(
            new QGenericItemModel(std::move(movedListOfObjects))
        );
    }) << 6 << 2 << (ChangeAction::ChangeRows | ChangeAction::SetData);

    // special case: tree
    if (tested & Trees) {
        QTest::addRow("value tree") << Factory([this]{
            return std::unique_ptr<QAbstractItemModel>(new QGenericItemModel(m_data->m_tree.get()));
        }) << int(std::size(*m_data->m_tree.get())) << int(std::tuple_size_v<tree_row>)
           << (ChangeAction::ChangeRows | ChangeAction::SetData);

        QTest::addRow("pointer tree") << Factory([this]{
            return std::unique_ptr<QAbstractItemModel>(
                new QGenericItemModel(m_data->m_pointer_tree.get(), tree_row::ProtocolPointerImpl{})
            );
        }) << int(std::size(*m_data->m_pointer_tree.get())) << int(std::tuple_size_v<tree_row>)
           << (ChangeAction::ChangeRows | ChangeAction::SetData);
    }
}

void tst_QGenericItemModel::basics()
{
#if QT_CONFIG(itemmodeltester)
    QFETCH(Factory, factory);
    auto model = factory();

    QAbstractItemModelTester modelTest(model.get(), this);
#else
    QSKIP("QAbstractItemModelTester not available");
#endif
}

void tst_QGenericItemModel::minimalIterator()
{
    struct Minimal
    {
        struct iterator
        {
            using value_type = QString;
            using size_type = int;
            using difference_type = int;
            using reference = value_type;
            using pointer = value_type;
            using iterator_category = std::forward_iterator_tag;

            constexpr iterator &operator++()
            { ++m_index; return *this; }
            constexpr iterator operator++(int)
            { auto copy = *this; ++m_index; return copy; }

            reference operator*() const
            { return QString::number(m_index); }
            constexpr bool operator==(const iterator &other) const noexcept
            { return m_index == other.m_index; }
            constexpr bool operator!=(const iterator &other) const noexcept
            { return m_index != other.m_index; }

            size_type m_index;
        };

#if defined (__cpp_concepts)
        static_assert(std::forward_iterator<iterator>);
#endif
        iterator begin() const { return iterator{0}; }
        iterator end() const { return iterator{m_size}; }

        int m_size;
    } minimal{100};

    QGenericItemModel model(minimal);
    QCOMPARE(model.rowCount(), minimal.m_size);
    for (int row = model.rowCount() - 1; row >= 0; --row) {
        const QModelIndex index = model.index(row, 0);
        QCOMPARE(index.data(), QString::number(row));
        QVERIFY(!index.flags().testFlag(Qt::ItemIsEditable));
    }
}

void tst_QGenericItemModel::ranges()
{
#if defined(__cpp_lib_ranges)
    const int lowest = 1;
    const int highest = 10;
    QGenericItemModel model(std::views::iota(lowest, highest));
    QCOMPARE(model.rowCount(), highest - lowest);
    QCOMPARE(model.columnCount(), 1);
#else
    QSKIP("C++ ranges library not available");
#endif
}

void tst_QGenericItemModel::json()
{
    QJsonDocument json = QJsonDocument::fromJson(R"([ "one", "two" ])");
    QVERIFY(json.isArray());
    QGenericItemModel model(json.array());
    QCOMPARE(model.rowCount(), 2);
    const QModelIndex index = model.index(1, 0);
    QVERIFY(index.isValid());
    QCOMPARE(index.data().toString(), "two");
}

void tst_QGenericItemModel::ownership()
{
    { // a list of pointers to objects
        Object *object = new Object;
        QPointer guard = object;
        std::vector<Object *> objects {
            object
        };
        { // model does not take ownership
            QGenericItemModel modelOnCopy(objects);
        }
        QVERIFY(guard);
        { // model does not take ownership
            QGenericItemModel modelOnRef(&objects);
        }
        QVERIFY(guard);
        { // model does take ownership
            QGenericItemModel movedIntoModel(std::move(objects));
        }
        QVERIFY(!guard);
    }

    { // a list of shared_ptr
        Object *object = new Object;
        QPointer guard = object;
        std::vector<std::shared_ptr<Object>> objects {
            std::shared_ptr<Object>(object)
        };
        { // model does not take ownership
            QGenericItemModel modelOnCopy(objects);
            QCOMPARE(modelOnCopy.rowCount(), 1);
            QCOMPARE(objects[0].use_count(), 2);
        }
        QCOMPARE(objects[0].use_count(), 1);
        { // model does not take ownership
            QGenericItemModel modelOnRef(&objects);
            QCOMPARE(objects[0].use_count(), 1);
        }
        QCOMPARE(objects[0].use_count(), 1);
        QVERIFY(guard);
        { // model owns the last shared copy
            QGenericItemModel movedIntoModel(std::move(objects));
        }
        QVERIFY(!guard);
    }

    { // a table of pointers
        Object *object = new Object;
        QPointer guard = object;
        std::vector<std::vector<Object *>> table {
            {object}
        };
        { // model does not take ownership
            QGenericItemModel modelOnCopy(table);
        }
        QVERIFY(guard);
        { // model does not take ownership
            QGenericItemModel modelOnRef(&table);
        }
        QVERIFY(guard);
        { // model does take ownership of rows, but not of objects within each row
            QGenericItemModel movedIntoModel(std::move(table));
        }
        QVERIFY(guard);
        delete object;
    }
}

void tst_QGenericItemModel::dimensions()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const int, expectedRowCount);
    QFETCH(const int, expectedColumnCount);

    QCOMPARE(model->rowCount(), expectedRowCount);
    QCOMPARE(model->columnCount(), expectedColumnCount);
}

void tst_QGenericItemModel::flags()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const ChangeActions, changeActions);

    const QModelIndex first = model->index(0, 0);
    QVERIFY(first.isValid());
    const QModelIndex last = model->index(model->rowCount() - 1, model->columnCount() - 1);
    QVERIFY(last.isValid());

    QCOMPARE(first.flags().testFlag(Qt::ItemIsEditable),
             changeActions.testFlags(ChangeAction::SetData));
    QCOMPARE(last.flags().testFlag(Qt::ItemIsEditable),
             changeActions.testFlags(ChangeAction::SetData));
}

void tst_QGenericItemModel::data()
{
    QFETCH(Factory, factory);
    auto model = factory();

    QVERIFY(!model->data({}).isValid());

    const QModelIndex first = model->index(0, 0);
    QVERIFY(first.isValid());
    const QModelIndex last = model->index(model->rowCount() - 1, model->columnCount() - 1);
    QVERIFY(last.isValid());

    QVERIFY(first.data().isValid());
    QVERIFY(last.data().isValid());
}

void tst_QGenericItemModel::setData()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const ChangeActions, changeActions);

    QVERIFY(!model->setData({}, {}));

    const QModelIndex first = model->index(0, 0);
    QVERIFY(first.isValid());

    QVariant newValue = 12345;
    const QVariant oldValue = first.data();
    QVERIFY(oldValue.isValid());

    if (!newValue.canConvert(oldValue.metaType()))
        newValue = QVariant(oldValue.metaType());
    QCOMPARE(first.data(), oldValue);
    QCOMPARE(model->setData(first, newValue), changeActions.testFlag(ChangeAction::SetData));
    QCOMPARE(first.data() == oldValue, !changeActions.testFlag(ChangeAction::SetData));
}

void tst_QGenericItemModel::itemData()
{
    QFETCH(Factory, factory);
    auto model = factory();

    QVERIFY(model->itemData({}).isEmpty());

    const QModelIndex index = model->index(0, 0);
    const QMap<int, QVariant> itemData = model->itemData(index);
    for (int role = 0; role < Qt::UserRole; ++role) {
        if (role == Qt::EditRole) // we fake that in data()
            continue;
        QCOMPARE(itemData.value(role), index.data(role));
    }
}

void tst_QGenericItemModel::setItemData()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const ChangeActions, changeActions);

    QVERIFY(!model->setItemData({}, {}));

    const QModelIndex index = model->index(0, 0);
    QMap<int, QVariant> itemData = model->itemData(index);
    // we only care about multi-role models
    const auto roles = itemData.keys();
    if (roles == QList<int>{Qt::DisplayRole, Qt::EditRole})
        QSKIP("Can't test setItemData on models with single values!");

    itemData = {};
    for (int role : roles) {
        if (role == Qt::EditRole) // faked
            continue;
        QVariant data = role != Qt::DecorationRole ? QVariant(QStringLiteral("Role %1").arg(role))
                                                   : QVariant(QColor(Qt::magenta));
        itemData.insert(role, data);
    }

    QCOMPARE_NE(model->itemData(index), itemData);
    QCOMPARE(model->setItemData(index, itemData),
             changeActions.testFlag(ChangeAction::SetItemData));
    if (!changeActions.testFlag(ChangeAction::SetItemData))
        return; // nothing more to test for those models

    {
        const auto newItemData = model->itemData(index);
        auto diagnostics = qScopeGuard([&]{
            qDebug() << "Mismatch";
            qDebug() << "     Actual:" << newItemData;
            qDebug() << "   Expected:" << itemData;
        });
        QCOMPARE(newItemData == itemData, changeActions.testFlag(ChangeAction::SetItemData));
        diagnostics.dismiss();
    }

    for (int role = 0; role < Qt::UserRole; ++role) {
        if (role == Qt::EditRole) // faked role
            continue;

        QVariant data = index.data(role);
        auto diagnostics = qScopeGuard([&]{
            qDebug() << "Mismatch for" << Qt::ItemDataRole(role);
            qDebug() << "     Actual:" << data;
            qDebug() << "   Expected:" << itemData.value(role);
        });
        QCOMPARE(data == itemData.value(role), changeActions.testFlag(ChangeAction::SetData));
        diagnostics.dismiss();
    }
}

void tst_QGenericItemModel::clearItemData()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const ChangeActions, changeActions);

    QVERIFY(!model->clearItemData({}));

    const QModelIndex index0 = model->index(1, 0);
    const QModelIndex index1 = model->index(1, 1);
    const QVariant oldDataAt0 = index0.data();
    const QVariant oldDataAt1 = index1.data();
    QCOMPARE(model->clearItemData(index0), changeActions.testFlags(ChangeAction::SetData));
    QCOMPARE(index0.data() == oldDataAt0, !changeActions.testFlags(ChangeAction::SetData));
    QCOMPARE(index1.data(), oldDataAt1);
}

void tst_QGenericItemModel::insertRows()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const int, expectedRowCount);
    QFETCH(const int, expectedColumnCount);
    QFETCH(const ChangeActions, changeActions);
    const bool canSetData = changeActions.testFlag(ChangeAction::SetData);

    const QList<QPersistentModelIndex> pmiList = allIndexes(model.get());

    QCOMPARE(model->rowCount(), expectedRowCount);
    QCOMPARE(model->insertRow(0), changeActions.testFlag(ChangeAction::InsertRows));
    QCOMPARE(model->rowCount() == expectedRowCount + 1,
             changeActions.testFlag(ChangeAction::InsertRows));

    auto ignoreFailureFromAssociativeContainers = [] {
        for (auto suffix : { "Pointer", "Copy", "Ref" }) {
            auto addCase = [suffix](const std::string& testName,
                                    const std::string& containerName) {
              QEXPECT_FAIL((testName + suffix).c_str(),
                           (containerName + " is empty by design").c_str(),
                           Continue);
            };
            addCase("listOfNamedRoles", "QVariantMap");
            addCase("tableOfEnumRoles", "QVariantMap");
            addCase("tableOfIntRoles", "QVariantMap");
            addCase("stdTableOfIntRoles", "std::map");
        }
    };
    // get and put data into the new row
    const QModelIndex firstItem = model->index(0, 0);
    const QModelIndex lastItem = model->index(0, expectedColumnCount - 1);
    QVERIFY(firstItem.isValid());
    QVERIFY(lastItem.isValid());
    const QVariant firstValue = firstItem.data();
    const QVariant lastValue = lastItem.data();
    QEXPECT_FAIL("tableOfPointersPointer", "No item created", Continue);
    QEXPECT_FAIL("tableOfRowPointersPointer", "No row created", Continue);
    QEXPECT_FAIL("listOfObjectsCopy", "No object created", Continue);
    QEXPECT_FAIL("listOfMetaObjectTupleCopy", "No object created", Continue);
    QEXPECT_FAIL("tableOfMetaObjectTupleCopy", "No object created", Continue);
    QEXPECT_FAIL("movedListOfObjects", "No object created", Continue);

    // associative containers are default constructed with no valid data
    ignoreFailureFromAssociativeContainers();

    QVERIFY(firstValue.isValid() && lastValue.isValid());
    ignoreFailureFromAssociativeContainers();
    QCOMPARE(model->setData(firstItem, lastValue), canSetData && lastValue.isValid());
    ignoreFailureFromAssociativeContainers();
    QCOMPARE(model->setData(lastItem, firstValue), canSetData && firstValue.isValid());

    // append more rows
    QCOMPARE(model->insertRows(model->rowCount(), 5),
             changeActions.testFlag(ChangeAction::InsertRows));
    QCOMPARE(model->rowCount() == expectedRowCount + 6,
             changeActions.testFlag(ChangeAction::InsertRows));

    verifyPmiList(pmiList);
}

void tst_QGenericItemModel::removeRows()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const int, expectedRowCount);
    QFETCH(const ChangeActions, changeActions);

    QCOMPARE(model->rowCount(), expectedRowCount);
    QCOMPARE(model->removeRow(0), changeActions.testFlag(ChangeAction::RemoveRows));
    QCOMPARE(model->rowCount() == expectedRowCount - 1,
             changeActions.testFlag(ChangeAction::RemoveRows));
    QCOMPARE(model->removeRows(model->rowCount() - 2, 2),
             changeActions.testFlag(ChangeAction::RemoveRows));
    QCOMPARE(model->rowCount() == expectedRowCount - 3,
             changeActions.testFlag(ChangeAction::RemoveRows));

    const int newRowCount = model->rowCount();
    // make sure we don't crash when removing more than exist
    const bool couldRemove = model->removeRows(model->rowCount() - 5, model->rowCount() * 2);
    QCOMPARE_LE(model->rowCount(), newRowCount);
    QCOMPARE(couldRemove, model->rowCount() != newRowCount);
}

void tst_QGenericItemModel::insertColumns()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const int, expectedColumnCount);
    QFETCH(const ChangeActions, changeActions);

    QCOMPARE(model->columnCount(), expectedColumnCount);
    QCOMPARE(model->insertColumn(0), changeActions.testFlag(ChangeAction::InsertColumns));
    QCOMPARE(model->columnCount() == expectedColumnCount + 1,
             changeActions.testFlag(ChangeAction::InsertColumns));

    // append
    QCOMPARE(model->insertColumns(model->columnCount(), 5),
             changeActions.testFlag(ChangeAction::InsertColumns));
    QCOMPARE(model->columnCount() == expectedColumnCount + 6,
             changeActions.testFlag(ChangeAction::InsertColumns));
}

void tst_QGenericItemModel::removeColumns()
{
    QFETCH(Factory, factory);
    auto model = factory();
    QFETCH(const int, expectedColumnCount);
    QFETCH(const ChangeActions, changeActions);

    QCOMPARE(model->columnCount(), expectedColumnCount);
    QCOMPARE(model->removeColumn(0),
             changeActions.testFlag(ChangeAction::RemoveColumns));
}

void tst_QGenericItemModel::inconsistentColumnCount()
{
    QTest::ignoreMessage(QtCriticalMsg, "QGenericItemModel: "
        "Column-range at row 1 is not large enough!");

    std::vector<std::vector<int>> fuzzyTable = {
        {0},
        {},
        {2},
    };
    QGenericItemModel model(fuzzyTable);
    QCOMPARE(model.columnCount(), 1);
    for (int row = 0; row < model.rowCount(); ++row) {
        auto debug = qScopeGuard([&]{
            qCritical() << "Test failed for row" << row << fuzzyTable.at(row).size();
        });
        const bool shouldWork = int(fuzzyTable.at(row).size()) >= model.columnCount();
        const auto index = model.index(row, model.columnCount() - 1);
        QCOMPARE(index.isValid(), shouldWork);
        // none of these should crash
        QCOMPARE(index.data().isValid(), shouldWork);
        QCOMPARE(model.setData(index, row + 5), shouldWork);
        QCOMPARE(model.clearItemData(index), shouldWork);
        debug.dismiss();
    }
}

enum class TreeProtocol { ValueImplicit, ValueReadOnly, PointerExplicit, PointerExplicitMoved };

void tst_QGenericItemModel::createTree()
{
    m_data->m_tree.reset(new value_tree{
        {"1", "one"},
        {"2", "two"},
        {"3", "three"},
        {"4", "four"},
        {"5", "five"},
    });

    (*m_data->m_tree)[1].addChild("2.1", "two.one");
    (*m_data->m_tree)[1].addChild("2.2", "two.two");

    tree_row &row23 = (*m_data->m_tree)[1].addChild("2.3", "two.three");

    row23.addChild("2.3.1", "two.three.one");
    row23.addChild("2.3.2", "two.three.two");
    row23.addChild("2.3.3", "two.three.three");

    // assert the integrity of the tree; this is not a test.
    Q_ASSERT(!m_data->m_tree->at(0).childRows());
    Q_ASSERT(m_data->m_tree->at(1).childRows());
    Q_ASSERT(!m_data->m_tree->at(1).childRows()->at(1).childRows());
    Q_ASSERT(m_data->m_tree->at(1).childRows()->at(2).childRows());

    m_data->m_pointer_tree.reset(new pointer_tree{
        new tree_row("1", "one"),
        new tree_row("2", "one"),
        new tree_row("3", "one"),
        new tree_row("4", "one"),
        new tree_row("5", "one"),
    });

    m_data->m_pointer_tree->at(1)->addChildPointer("2.1", "two.one");
    m_data->m_pointer_tree->at(1)->addChildPointer("2.2", "two.two");
}

void tst_QGenericItemModel::tree_data()
{
    m_data.reset(new Data);
    createTree();

    QTest::addColumn<TreeProtocol>("protocol");
    QTest::addColumn<int>("expectedRootRowCount");
    QTest::addColumn<int>("expectedColumnCount");
    QTest::addColumn<QList<int>>("rowsWithChildren");
    QTest::addColumn<ChangeActions>("changeActions");

    const int expectedRootRowCount = int(m_data->m_tree->size());
    const int expectedColumnCount = int(std::tuple_size_v<tree_row>);
    const auto rowsWithChildren = QList{1};

    QTest::addRow("ValueImplicit")
        << TreeProtocol::ValueImplicit
        << expectedRootRowCount << expectedColumnCount << rowsWithChildren
        << ChangeActions(ChangeAction::All);
    QTest::addRow("ValueReadOnly")
        << TreeProtocol::ValueReadOnly
        << expectedRootRowCount << expectedColumnCount << rowsWithChildren
        << ChangeActions(ChangeAction::ReadOnly);
    QTest::addRow("PointerExplicit")
        << TreeProtocol::PointerExplicit
        << expectedRootRowCount << expectedColumnCount << rowsWithChildren
        << ChangeActions(ChangeAction::All);
    QTest::addRow("PointerExplicitMoved")
        << TreeProtocol::PointerExplicitMoved
        << expectedRootRowCount << expectedColumnCount << rowsWithChildren
        << ChangeActions(ChangeAction::All);
}

std::unique_ptr<QAbstractItemModel> tst_QGenericItemModel::makeTreeModel()
{
    createTree();

    std::unique_ptr<QAbstractItemModel> model;

    QFETCH(const TreeProtocol, protocol);
    switch (protocol) {
    case TreeProtocol::ValueImplicit:
        model.reset(new QGenericItemModel(m_data->m_tree.get()));
        break;
    case TreeProtocol::ValueReadOnly: {
        struct { // minimal (read-only) implementation of the tree traversal protocol
            const tree_row *parentRow(const tree_row &row) const { return row.parentRow(); }
            const std::optional<value_tree> &childRows(const tree_row &row) const
            { return row.childRows(); }
        } readOnlyProtocol;
        model.reset(new QGenericItemModel(m_data->m_tree.get(), readOnlyProtocol));
        break;
    }
    case TreeProtocol::PointerExplicit:
        model.reset(new QGenericItemModel(m_data->m_pointer_tree.get(),
                    tree_row::ProtocolPointerImpl{}));
        break;
    case TreeProtocol::PointerExplicitMoved: {
        pointer_tree moved_tree{
            new tree_row("m1", "m_one"),
            new tree_row("m2", "m_two"),
            new tree_row("m3", "m_three"),
            new tree_row("m4", "m_four"),
            new tree_row("m5", "m_five"),
        };
        moved_tree.at(1)->addChildPointer("2.1", "two.one");
        moved_tree.at(1)->addChildPointer("2.2", "two.two");

        model.reset(new QGenericItemModel(std::move(moved_tree),
                    tree_row::ProtocolPointerImpl{}));
        break;
    }
    }

    return model;
}

void tst_QGenericItemModel::tree()
{
    auto model = makeTreeModel();
    QFETCH(const int, expectedRootRowCount);
    QFETCH(const int, expectedColumnCount);
    QFETCH(QList<int>, rowsWithChildren);

    QCOMPARE(model->rowCount(), expectedRootRowCount);
    QCOMPARE(model->columnCount(), expectedColumnCount);

    for (int row = 0; row < model->rowCount(); ++row) {
        const bool expectedChildren = rowsWithChildren.contains(row);
        const QModelIndex parent = model->index(row, 0);
        QVERIFY(parent.isValid());
        QCOMPARE(model->hasChildren(parent), expectedChildren);
        if (expectedChildren)
            QCOMPARE_GT(model->rowCount(parent), 0);
        else
            QCOMPARE(model->rowCount(parent), 0);
        QCOMPARE(model->columnCount(parent), expectedColumnCount);
        const QModelIndex child = model->index(0, 0, parent);
        QCOMPARE(child.isValid(), expectedChildren);
        if (expectedChildren)
            QCOMPARE(child.parent(), parent);
        else
            QCOMPARE(child.parent(), QModelIndex());
    }

#if QT_CONFIG(itemmodeltester)
    QAbstractItemModelTester modelTest(model.get());
#endif
}

void tst_QGenericItemModel::treeModifyBranch()
{
    auto model = makeTreeModel();
    QFETCH(QList<int>, rowsWithChildren);
    QFETCH(const ChangeActions, changeActions);

    int rowWithChildren = rowsWithChildren.first();
    QCOMPARE_GT(rowWithChildren, 0);

    // removing or inserting a row adjusts the parents of the direct children
    // of the following branches
    {
        QVERIFY(treeIntegrityCheck());
        QCOMPARE(model->removeRow(--rowWithChildren),
                 changeActions.testFlag(ChangeAction::RemoveRows));
        QVERIFY(treeIntegrityCheck());
        QCOMPARE(model->insertRow(rowWithChildren++),
                 changeActions.testFlag(ChangeAction::InsertRows));
        QVERIFY(treeIntegrityCheck());
        if (!changeActions.testFlag(ChangeAction::ChangeRows))
            return; // nothing else to test with a read-only model
    }

    const QModelIndex parent = model->index(rowWithChildren, 0);
    int oldRowCount = model->rowCount(parent);

    // append
    {
        QVERIFY(model->insertRow(oldRowCount, parent));
        QModelIndex newChild = model->index(oldRowCount, 0, parent);
        QVERIFY(newChild.isValid());
        QCOMPARE(model->rowCount(parent), ++oldRowCount);
        QCOMPARE(newChild.parent(), parent);
    }

    // prepend
    {
        QVERIFY(model->insertRow(0, parent));
        QModelIndex newChild = model->index(0, 0, parent);
        QVERIFY(newChild.isValid());
        QCOMPARE(model->rowCount(parent), ++oldRowCount);
        QCOMPARE(newChild.parent(), parent);
    }

    // remove last
    {
        QVERIFY(model->removeRow(model->rowCount(parent) - 1, parent));
        QCOMPARE(model->rowCount(parent), --oldRowCount);
    }

    // remove first
    {
        QVERIFY(model->rowCount(parent) > 0);
        QVERIFY(model->removeRow(0, parent));
        QCOMPARE(model->rowCount(parent), --oldRowCount);
    }

#if QT_CONFIG(itemmodeltester)
    QAbstractItemModelTester modelTest(model.get());
#endif
}

void tst_QGenericItemModel::treeCreateBranch()
{
    auto model = makeTreeModel();
    QFETCH(QList<int>, rowsWithChildren);
    QFETCH(const ChangeActions, changeActions);

#if QT_CONFIG(itemmodeltester)
    QAbstractItemModelTester modelTest(model.get());
#endif

    const QList<QPersistentModelIndex> pmiList = allIndexes(model.get());

    // new branch
    QVERIFY(!rowsWithChildren.contains(0));
    const QModelIndex parent = model->index(0, 0);
    QVERIFY(!model->hasChildren(parent));
    QCOMPARE(model->insertRows(0, 5, parent),
             changeActions.testFlag(ChangeAction::InsertRows));
    if (!changeActions.testFlag(ChangeAction::InsertRows))
        return; // nothing else to test with a read-only model
    QVERIFY(model->hasChildren(parent));
    QCOMPARE(model->rowCount(parent), 5);

    for (int i = 0; i < model->rowCount(parent); ++i) {
        QModelIndex newChild = model->index(i, 0, parent);
        QVERIFY(newChild.isValid());
        QCOMPARE(newChild.parent(), parent);
        QVERIFY(!model->hasChildren(newChild));
    }

    verifyPmiList(pmiList);
}

void tst_QGenericItemModel::treeRemoveBranch()
{
    auto model = makeTreeModel();
    QFETCH(QList<int>, rowsWithChildren);
    QFETCH(const ChangeActions, changeActions);

#if QT_CONFIG(itemmodeltester)
    QAbstractItemModelTester modelTest(model.get());
#endif

    const QModelIndex parent = model->index(rowsWithChildren.first(), 0);
    QVERIFY(parent.isValid());
    QVERIFY(model->hasChildren(parent));
    const int oldRowCount = model->rowCount(parent);
    QCOMPARE_GT(oldRowCount, 0);

    // out of bounds asserts in QAIM::removeRows
    // QVERIFY(model->removeRows(0, oldRowCount * 2, parent));

    QCOMPARE(model->removeRows(0, oldRowCount, parent),
             changeActions.testFlag(ChangeAction::RemoveRows));
    if (!changeActions.testFlag(ChangeAction::RemoveRows))
        return; // nothing else to test with a read-only model
    QVERIFY(!model->hasChildren(parent));
    QCOMPARE(model->rowCount(parent), 0);
}

QTEST_MAIN(tst_QGenericItemModel)
#include "tst_qgenericitemmodel.moc"
