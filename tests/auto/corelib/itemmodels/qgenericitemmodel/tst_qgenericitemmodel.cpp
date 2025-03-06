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

class tst_QGenericItemModel : public QObject
{
    Q_OBJECT

private slots:
    void basics_data() { createTestData(); }
    void basics();
    void minimalIterator();
    void ranges();
    void json();

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

private:
    void createTestData();

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

void tst_QGenericItemModel::createTestData()
{
    m_data.reset(new Data);

    QTest::addColumn<Factory>("factory");
    QTest::addColumn<int>("expectedRowCount");
    QTest::addColumn<int>("expectedColumnCount");
    QTest::addColumn<ChangeActions>("changeActions");

    Factory factory;

#define ADD_HELPER(Model, Tag, Ref) \
    factory = [this]() -> std::unique_ptr<QAbstractItemModel> { \
        return std::unique_ptr<QAbstractItemModel>(new QGenericItemModel(Ref->Model)); \
    }; \
    QTest::addRow(#Model #Tag) << factory << int(std::size(m_data->Model)) \

#define ADD_POINTER(Model) \
    ADD_HELPER(Model, Pointer, &m_data) \

#define ADD_COPY(Model) \
    ADD_HELPER(Model, Copy, m_data) \

    // POINTER-tests will modify the data structure that lives in m_data,
    // so we have to run tests on copies of that data first for each type,
    // or only run POINTER-tests.
    // The entire test data is recreated for each test function, but test
    // functions must not change data structures other than the one tested.

    ADD_COPY(fixedArrayOfNumbers)
        << 1 << ChangeActions(ChangeAction::SetData);
    ADD_POINTER(fixedArrayOfNumbers)
        << 1 << ChangeActions(ChangeAction::SetData);
    ADD_POINTER(cArrayOfNumbers)
        << 1 << ChangeActions(ChangeAction::SetData);

    ADD_POINTER(cArrayFixedColumns)
        << int(std::tuple_size_v<Row>) << (ChangeAction::SetData | ChangeAction::SetItemData);

    ADD_COPY(vectorOfFixedColumns)
        << 2 << (ChangeAction::ChangeRows | ChangeAction::SetData);
    ADD_POINTER(vectorOfFixedColumns)
        << 2 << (ChangeAction::ChangeRows | ChangeAction::SetData);
    ADD_COPY(vectorOfArrays)
        << 10 << (ChangeAction::ChangeRows | ChangeAction::SetData);
    ADD_POINTER(vectorOfArrays)
        << 10 << (ChangeAction::ChangeRows | ChangeAction::SetData);
    ADD_COPY(vectorOfStructs)
        << int(std::tuple_size_v<Row>) << (ChangeAction::ChangeRows | ChangeAction::SetData
                                                                    | ChangeAction::SetItemData);
    ADD_POINTER(vectorOfStructs)
        << int(std::tuple_size_v<Row>) << (ChangeAction::ChangeRows | ChangeAction::SetData
                                                                    | ChangeAction::SetItemData);
    ADD_COPY(vectorOfConstStructs)
        << int(std::tuple_size_v<ConstRow>) << ChangeActions(ChangeAction::ChangeRows);
    ADD_POINTER(vectorOfConstStructs)
        << int(std::tuple_size_v<ConstRow>) << ChangeActions(ChangeAction::ChangeRows);

    ADD_COPY(vectorOfGadgets)
        << 3 << (ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);
    ADD_POINTER(vectorOfGadgets)
        << 3 << (ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);
    ADD_COPY(listOfGadgets)
        << 1 << (ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);
    ADD_POINTER(listOfGadgets)
        << 1 << (ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);
    ADD_COPY(listOfObjects)
        << 2 << (ChangeAction::ChangeRows | ChangeAction::SetData);

    ADD_COPY(tableOfNumbers)
        << 5 << ChangeActions(ChangeAction::All);
    ADD_POINTER(tableOfNumbers)
        << 5 << ChangeActions(ChangeAction::All);
    // only adding as pointer, copy would operate on the same data
    ADD_POINTER(tableOfPointers)
        << 2 << ChangeActions(ChangeAction::All | ChangeAction::SetItemData);
    ADD_POINTER(tableOfRowPointers)
        << int(std::tuple_size_v<Row>) << (ChangeAction::ChangeRows | ChangeAction::SetData
                                                                    | ChangeAction::SetItemData);

    ADD_COPY(arrayOfConstNumbers)
        << 1 << ChangeActions(ChangeAction::ReadOnly);
    ADD_POINTER(arrayOfConstNumbers)
        << 1 << ChangeActions(ChangeAction::ReadOnly);

    ADD_COPY(constListOfNumbers)
        << 1 << ChangeActions(ChangeAction::ReadOnly);
    ADD_POINTER(constListOfNumbers)
        << 1 << ChangeActions(ChangeAction::ReadOnly);

    ADD_COPY(constTableOfNumbers)
        << 5 << ChangeActions(ChangeAction::ReadOnly);
    ADD_POINTER(constTableOfNumbers)
        << 5 << ChangeActions(ChangeAction::ReadOnly);

    ADD_COPY(listOfNamedRoles)
        << 1 << (ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);
    ADD_POINTER(listOfNamedRoles)
        << 1 << (ChangeAction::ChangeRows | ChangeAction::SetData | ChangeAction::SetItemData);
    ADD_COPY(tableOfEnumRoles)
        << 1 << ChangeActions(ChangeAction::All | ChangeAction::SetItemData);
    ADD_POINTER(tableOfEnumRoles)
        << 1 << ChangeActions(ChangeAction::All | ChangeAction::SetItemData);
    ADD_COPY(tableOfIntRoles)
        << 1 << ChangeActions(ChangeAction::All | ChangeAction::SetItemData);
    ADD_POINTER(tableOfIntRoles)
        << 1 << ChangeActions(ChangeAction::All | ChangeAction::SetItemData);
    ADD_COPY(stdTableOfIntRoles)
        << 1 << ChangeActions(ChangeAction::All | ChangeAction::SetItemData);
    ADD_POINTER(stdTableOfIntRoles)
        << 1 << ChangeActions(ChangeAction::All | ChangeAction::SetItemData);

#undef ADD_COPY
#undef ADD_POINTER
#undef ADD_HELPER

    QTest::addRow("Moved table") << Factory([]{
        QList<std::vector<QString>> movedTable = {
            {"0/0", "0/1", "0/2", "0/3"},
            {"1/0", "1/1", "1/2", "1/3"},
            {"2/0", "2/1", "2/2", "2/3"},
            {"3/0", "3/1", "3/2", "3/3"},
        };
        return std::unique_ptr<QAbstractItemModel>(new QGenericItemModel(std::move(movedTable)));
    }) << 4 << 4 << ChangeActions(ChangeAction::All);
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

    QCOMPARE(model->rowCount(), expectedRowCount);
    QCOMPARE(model->insertRow(0), changeActions.testFlag(ChangeAction::InsertRows));
    QCOMPARE(model->rowCount() == expectedRowCount + 1,
             changeActions.testFlag(ChangeAction::InsertRows));

    auto ignoreFailureFromAssociativeContainers = []{
        QEXPECT_FAIL("listOfNamedRolesPointer", "QVariantMap is empty by design", Continue);
        QEXPECT_FAIL("listOfNamedRolesCopy", "QVariantMap is empty by design", Continue);
        QEXPECT_FAIL("tableOfEnumRolesPointer", "QVariantMap is empty by design", Continue);
        QEXPECT_FAIL("tableOfEnumRolesCopy", "QVariantMap is empty by design", Continue);
        QEXPECT_FAIL("tableOfIntRolesPointer", "QVariantMap is empty by design", Continue);
        QEXPECT_FAIL("tableOfIntRolesCopy", "QVariantMap is empty by design", Continue);
        QEXPECT_FAIL("stdTableOfIntRolesPointer", "std::map is empty by design", Continue);
        QEXPECT_FAIL("stdTableOfIntRolesCopy", "std::map is empty by design", Continue);
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

QTEST_MAIN(tst_QGenericItemModel)
#include "tst_qgenericitemmodel.moc"
