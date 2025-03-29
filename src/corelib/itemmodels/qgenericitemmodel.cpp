// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qgenericitemmodel.h"

QT_BEGIN_NAMESPACE

/*!
    \class QGenericItemModel
    \inmodule QtCore
    \since 6.10
    \ingroup model-view
    \brief QGenericItemModel implements QAbstractItemModel for any C++ range.
    \reentrant

    QGenericItemModel can make the data in any sequentially iterable C++ type
    available to the \l{Model/View Programming}{model/view framework} of Qt.
    This makes it easy to display existing data structures in the Qt Widgets
    and Qt Quick item views, and to allow the user of the application to
    manipulate the data using a graphical user interface.

    To use QGenericItemModel, instantiate it with a C++ range and set it as
    the model of one or more views:

    \snippet qgenericitemmodel/main.cpp array

    The range can be any C++ type for which the standard methods
    \c{std::cbegin} and \c{std::cend} are implemented, and for which the
    returned iterator type satisfies \c{std::forward_iterator}. Certain model
    operations will perform better if \c{std::size} is available, and if the
    iterator satisfies \c{std::random_access_iterator}.

    The range can be provided by pointer or by value, and has to be provided
    when constructing the model. If the range is provided by pointer, then
    QAbstractItemModel APIs that modify the model, such as setData() or
    insertRows(), modify the range. The caller must make sure that the
    range's lifetime exceeds the lifetime of the model. Methods that modify
    the structure of the range, such as insertRows() or removeColumns(), use
    standard C++ container APIs \c{resize()}, \c{insert()}, \c{erase()}, in
    addition to dereferencing a mutating iterator to set or clear the data.

    There is no API to retrieve the range again, so constructing the model
    from a range by value is mostly only useful for displaying data.
    Changes to the data can be monitored using the signals emitted by the
    model, such as \l{QAbstractItemModel}{dataChanged()}.

    \section2 Read-only or mutable

    For ranges that are const objects, for which access always yields
    constant values, or where the required container APIs are not available,
    QGenericItemModel implements write-access APIs to do nothing and return
    \c{false}. In the example above, the model cannot add or remove rows, as
    the number of entries in a C++ array is fixed. But the values can be
    changed using setData(), and the user can trigger editing of the values in
    the list view. By making the array const, the values also become read-only.

    \snippet qgenericitemmodel/main.cpp const_array

    The values are also read-only if the element type is const, like in

    \snippet qgenericitemmodel/main.cpp const_values

    \note If the values in the range are const, then it's also not possible
    to remove or insert columns and rows through the QAbstractItemModel API.
    For more granular control, implement \l{the C++ tuple protocol}.

    \section1 List, Table, or Tree

    The elements in the range are interpreted as rows of the model. Depending
    on the type of these rows, QGenericItemModel exposes the range as a list,
    a table, or a tree.

    If the row type is not an iterable range, and does not implement the
    C++ tuple protocol, then the range gets represented as a list.

    \snippet qgenericitemmodel/main.cpp list_of_int

    If the row type is an iterable range, then the range gets represented as a
    table.

    \snippet qgenericitemmodel/main.cpp grid_of_numbers

    With such a row type, the number of columns can be changed via
    insertColumns() and removeColumns(). However, all rows are expected to have
    the same number of columns.

    \section2 Fixed-size rows

    If the row type implements \l{the C++ tuple protocol}, then the range gets
    represented as a table with a fixed number of columns.

    \snippet qgenericitemmodel/main.cpp pair_int_QString

    An easier and more flexible alternative to implementing the tuple protocol
    for a C++ type is to use Qt's \l{Meta-Object System}{meta-object system} to
    declare a type with \l{Qt's Property System}{properties}. This can be a
    value type that is declared as a \l{Q_GADGET}{gadget}, or a QObject subclass.

    \snippet qgenericitemmodel/main.cpp gadget

    Using QObject subclasses allows properties to be \l{Qt Bindable Properties}
    {bindable}, or to have change notification signals. However, using QObject
    instances for items has significant memory overhead.

    Using Qt gadgets or objects is more convenient and can be more flexible
    than implementing the tuple protocol. Those types are also directly
    accessible from within QML. However, the access through the property system
    comes with some runtime overhead. For performance critical models, consider
    implementing the tuple protocol for compile-time generation of the access
    code.

    \section2 Trees of data

    QGenericItemModel can represent a data structure as a tree model. Such a
    tree data structure needs to be homomorphic: on all levels of the tree, the
    list of child rows needs to use the exact same representation as the tree
    itself. In addition, the row type needs be of a static size: either a gadget
    or QObject type, or a type that implements the {C++ tuple protocol}.

    To represent such data as a tree, the row type has to implement a traversal
    protocol that allows QGenericItemModel to navigate up and down the tree.
    For any given row, the model needs to be able to retrieve the parent row,
    and the span of children for any given row.

    \snippet qgenericitemmodel/main.cpp tree_protocol_0

    The tree itself is a vector of \c{TreeRow} values. See \l{Rows as pointers
    or values} for the considerations on whether to use values or pointers of
    items for the rows.

    \snippet qgenericitemmodel/main.cpp tree_protocol_1

    The row class can be of any fixed-size type described above: a type that
    implements the tuple protocol, a gadget, or a QObject. In this example, we
    use a gadget.

    Each row item needs to maintain a pointer to the parent row, as well as an
    optional range of child rows that is identical to the structure used for the
    tree.

    Making the row type default constructible is optional, and allows the model
    to construct new row data elements, for instance in the insertRow() or
    moveRows() implementations.

    \snippet qgenericitemmodel/main.cpp tree_protocol_2

    The tree traversal protocol can then be implemented as member functions of
    the row data type. A const \c{parentRow()} function has to return a pointer
    to a const row item; and the \c{childRows()} function has to return a
    reference to a const \c{std::optional} that can hold the optional child
    range.

    These two functions are sufficient for the model to navigate the tree as a
    read-only data structure. To allow the user to edit data in a view, and the
    model to implement mutating model APIs such as insertRows(), removeRows(),
    and moveRows(), we have to implement additional functions for write-access:

    \snippet qgenericitemmodel/main.cpp tree_protocol_3

    The model calls the \c{setParentRow()} function and mutable \c{childRows()}
    overload to move or insert rows into an existing tree branch, and to update
    the parent pointer should the old value have become invalid. The non-const
    overload of \c{childRows()} provides in addition write-access to the row
    data.

    \note The model performs setting the parent of a row, removing that row
    from the old parent, and adding it to the list of the new parent's children,
    as separate steps. This keeps the protocol interface small.

    \dots
    \snippet qgenericitemmodel/main.cpp tree_protocol_4

    The rest of the class implementation is not relevant for the model, but
    a \c{addChild()} helper provides us with a convenient way to construct the
    initial state of the tree.

    \snippet qgenericitemmodel/main.cpp tree_protocol_5

    A QGenericItemModel instantiated with an instance of such a range will
    represent the data as a tree.

    \snippet qgenericitemmodel/main.cpp tree_protocol_6

    \section3 Tree traversal protocol in a separate class

    The tree traversal protocol can also be implemented in a separate class.

    \snippet qgenericitemmodel/main.cpp explicit_tree_protocol_0

    Pass an instance of this protocol implementation to the QGenericItemModel
    constructor:

    \snippet qgenericitemmodel/main.cpp explicit_tree_protocol_1

    \section2 Rows as pointers or values

    The row type of the data range can be either a value, or a pointer. In
    the code above we have been using the tree rows as values in a vector,
    which avoids that we have to deal with explicit memory management. However,
    a vector as a contiguous block of memory invalidates all iterators and
    references when it has to reallocate the storage, or when inserting or
    removing elements. This impacts the pointer to the parent item, which is
    the location of the parent row within the vector. Making sure that this
    parent (and QPersistentModelIndex instances referring to items within it)
    stays valid can incurr substantial performance overhead. The
    QGenericItemModel implementation has to assume that all references into the
    range become invalid when modifying the range.

    Alternatively, we can also use a range of row pointers as the tree type:

    \snippet qgenericitemmodel/main.cpp tree_of_pointers_0

    In this case, we have to allocate all TreeRow instances explicitly using
    operator \c{new}, and implement the destructor to \c{delete} all items in
    the vector of children.

    \snippet qgenericitemmodel/main.cpp tree_of_pointers_1
    \snippet qgenericitemmodel/main.cpp tree_of_pointers_2

    Before we can construct a model that represents this data as a tree, we need
    to also implement the tree traversal protocol.

    \snippet qgenericitemmodel/main.cpp tree_of_pointers_3

    An explicit protocol implementation for mutable trees of pointers has to
    provide two additional member functions, \c{newRow()} and
    \c{deleteRow(RowType *)}.

    \snippet qgenericitemmodel/main.cpp tree_of_pointers_4

    The model will call those functions when creating new rows in insertRows(),
    and when removing rows in removeRows(). In addition, if the model has
    ownership of the data, then it will also delete all top-level rows upon
    destruction. Note how in this example, we move the tree into the model, so
    we must no longer perform any operations on it. QGenericItemModel, when
    constructed by moving tree-data with row-pointers into it, will take
    ownership of the data, and delete the row pointers in it's destructor.

    \note This is not the case for tables and lists that use pointers as their
    row type. QGenericItemModel will never allocate new rows in lists and tables
    using operator new, and will never free any rows.

    So, using pointers at rows comes with some memory allocation and management
    overhead. However, when using rows through pointers the references to the
    row items remain stable, even when they are moved around in the range,
    or when the range reallocates. This can significantly reduce the cost
    of making modifications to the model's structure when using insertRows(),
    removeRows(), or moveRows().

    So, each choice has different performance and memory overhead trade-offs.
    The best option depends on the exact use case and data structure used.

    \section2 Multi-role items

    The type of the items that the implementations of data(), setData(),
    clearItemData() etc. operate on can be the same across the entire model -
    like in the \c{gridOfNumbers} example above. But the range can also have
    different item types for different columns, like in the \c{numberNames}
    case.

    By default, the value gets used for the Qt::DisplayRole and Qt::EditRole
    roles. Most views expect the value to be
    \l{QVariant::canConvert}{convertible to and from a QString} (but a custom
    delegate might provide more flexibility).

    If the item is an associative container that uses \c{int},
    \l{Qt::ItemDataRole}, or QString as the key type, and QVariant as the
    mapped type, then QGenericItemModel interprets that container as the storage
    of the data for multiple roles. The data() and setData() functions return
    and modify the mapped value in the container, and setItemData() modifies all
    provided values, itemData() returns all stored values, and clearItemData()
    clears the entire container.

    \snippet qgenericitemmodel/main.cpp color_map

    The most efficient data type to use as the key is Qt::ItemDataRole or
    \c{int}. When using \c{int}, itemData() returns the container as is, and
    doesn't have to create a copy of the data.

    Gadgets and QObject types are also represented at multi-role items if they
    are the item type in a table. The names of the properties have to match the
    names of the roles.

    \snippet qgenericitemmodel/main.cpp color_gadget_0

    When used in a list, these types are ambiguous: they can be represented as
    multi-column rows, with each property represented as a separate column. Or
    they can be single items with each property being a role. To disambiguate,
    use the QGenericItemModel::SingleColumn wrapper.

    \snippet qgenericitemmodel/main.cpp color_gadget_1

    \section2 The C++ tuple protocol

    As seen in the \c{numberNames} example above, the row type can be a tuple,
    and in fact any type that implements the tuple protocol. This protocol is
    implemented by specializing \c{std::tuple_size} and \c{std::tuple_element},
    and overloading the unqualified \c{get} function. Do so for your custom row
    type to make existing structured data available to the model/view framework
    in Qt.

    \snippet qgenericitemmodel/main.cpp tuple_protocol

    In the above implementation, the \c{title} and \c{author} values of the
    \c{Book} type are returned as \c{const}, so the model flags items in those
    two columns as read-only. The user won't be able to trigger editing, and
    setData() does nothing and returns false. For \c{summary} and \c{rating}
    the implementation returns the same value category as the book, so when
    \c{get} is called with a mutable reference to a \c{Book}, then it will
    return a mutable reference of the respective variable. The model makes
    those columns editable, both for the user and for programmatic access.

    \note The implementation of \c{get} above requires C++23. A C++17 compliant
    implementation can be found in the unit test code for QGenericItemModel.

    Types that have a meta objects, and implement the C++ tuple protocol, also
    can cause compile-time ambiguity when used as the row type, as the framework
    won't know which API to use to access the individual values. Use the
    QGenericItemModel::SingleColumn and QGenericItemModel::MultiColumns wrapper
    to disambiguate.

    \sa {Model/View Programming}
*/

/*!
    \typedef QGenericItemModel::SingleColumn

    Use this type to disambiguate when using the type \c{T} as the row type in
    the range. If \c{T} provides a metaobject, then the framework will by
    default represent the type as multiple columns, resulting in a table model.

    \snippet qgenericitemmodel/main.cpp color_gadget_0

    When stored in a sequential range, this type will be interpreted as
    multi-column rows with each property being one column. The range will be
    represented as a table.

    \code
    QList<ColorEntry> colors = {
        // ...
    };
    QGenericItemModel tableModel(colors); // columnCount() == 3
    \endcode

    When wrapped into QGenericItemModel::SingleColumn, the model will be a list,
    with each instance of \c{T} represented as an item with multiple roles.

    \code
    QList<QGenericItemModel::SingleColumn<ColorEntry>> colors = {
        // ...
    };
    QGenericItemModel listModel(colors); // columnCount() == 1
    \endcode

    \sa QGenericItemModel::MultiColumn
*/

/*!
    \class QGenericItemModel::MultiColumn
    \brief Represents the wrapped type \c{T} as multiple columns in a QGenericItemModel.
    \inmodule QtCore
    \ingroup model-view
    \since 6.10

    Use this type to disambiguate when the type \c{T} has both a metaobject, and
    implements \l{the C++ tuple protocol}. The type will be represented as
    multiple columns, and the individual values will be accessed through the
    tuple protocol.

    \snippet qgenericitemmodel/main.cpp color_gadget_0
    \code
    namespace std {
        template <> struct tuple_size<ColorEntry> : integral_constant<size_t, 3> {};
        // ...
    }

    QList<QGenericItemModel::MultiColumn<ColorEntry>> colors = {
        // ...
    };
    QGenericItemModel colorList(colors);
    \endcode

    To represent the type a single column value with multiple roles, use
    QGenericItemModel::SingleColumn instead.

    \sa QGenericItemModel::SingleColumn
*/

/*!
    \fn template <typename Range, QGenericItemModelDetails::if_is_table_range<Range>> QGenericItemModel::QGenericItemModel(Range &&range, QObject *parent)
    \fn template <typename Range, QGenericItemModelDetails::if_is_tree_range<Range>> QGenericItemModel::QGenericItemModel(Range &&range, QObject *parent)
    \fn template <typename Range, typename Protocol, QGenericItemModelDetails::if_is_tree_range<Range, Protocol>> QGenericItemModel::QGenericItemModel(Range &&range, Protocol &&protocol, QObject *parent)

    Constructs a generic item model instance that operates on the data in \a
    range. The \a range has to be a sequential range for which \c{std::cbegin}
    and \c{std::cend} are available. If \a protocol is provided, then the model
    will represent the range as a tree using the protocol implementation.
    The model instance becomes a child of \a parent.

    The \a range can be a pointer, in which case mutating model APIs will
    modify the data in that range instance. If \a range is a value (or moved
    into the model), then use the signals emitted by the model to respond to
    changes to the data.

    \note While the model does not take ownership of the range object, you
    must not modify the \a range directly once the model has been
    constructed. Such modifications will not emit signals necessary to keep
    model users (other models or views) synchronized with the model, resulting
    in inconsistent results, undefined behavior, and crashes.
*/

/*!
    Destroys the generic item model.

    The range that the model was constructed from is not destroyed.
*/
QGenericItemModel::~QGenericItemModel() = default;

/*!
    \reimp

    Returns the index of the model item at \a row and \a column in \a parent.

    Passing a valid parent produces an invalid index for models that operate on
    list and table ranges.

    \sa parent()
*/
QModelIndex QGenericItemModel::index(int row, int column, const QModelIndex &parent) const
{
    return impl->callConst<QModelIndex>(QGenericItemModelImplBase::Index, row, column, parent);
}

/*!
    \reimp

    Returns the parent of the item at the \a child index.

    This function always produces an invalid index for models that operate on
    list and table ranges. For models operation on a tree, this function
    returns the index for the row item returned by the parent() implementation
    of the tree traversal protocol.

    \sa index(), hasChildren()
*/
QModelIndex QGenericItemModel::parent(const QModelIndex &child) const
{
    return impl->callConst<QModelIndex>(QGenericItemModelImplBase::Parent, child);
}

/*!
    \reimp

    Returns the number of rows under the given \a parent. This is the number of
    items in the root range for an invalid \a parent index.

    If the \a parent index is valid, then this function always returns 0 for
    models that operate on list and table ranges. For trees, this returns the
    size of the range returned by the childRows() implementation of the tree
    traversal protocol.

    \sa columnCount(), insertRows(), hasChildren()
*/
int QGenericItemModel::rowCount(const QModelIndex &parent) const
{
    return impl->callConst<int>(QGenericItemModelImplBase::RowCount, parent);
}

/*!
    \reimp

    Returns the number of columns of the model. This function returns the same
    value for all \a parent indexes.

    For models operating on a statically sized row type, this returned value is
    always the same throughout the lifetime of the model. For models operating
    on dynamically sized row type, the model returns the number of items in the
    first row, or 0 if the model has no rows.

    \sa rowCount, insertColumns()
*/
int QGenericItemModel::columnCount(const QModelIndex &parent) const
{
    return impl->callConst<int>(QGenericItemModelImplBase::ColumnCount, parent);
}

/*!
    \reimp

    Returns the item flags for the given \a index.

    The implementation returns a combination of flags that enables the item
    (\c ItemIsEnabled) and allows it to be selected (\c ItemIsSelectable). For
    models operating on a range with mutable data, it also sets the flag
    that allows the item to be editable (\c ItemIsEditable).

    \sa Qt::ItemFlags
*/
Qt::ItemFlags QGenericItemModel::flags(const QModelIndex &index) const
{
    return impl->callConst<Qt::ItemFlags>(QGenericItemModelImplBase::Flags, index);
}

/*!
    \reimp

    Returns the data for the given \a role and \a section in the header with
    the specified \a orientation.

    For horizontal headers, the section number corresponds to the column
    number. Similarly, for vertical headers, the section number corresponds to
    the row number.

    \sa Qt::ItemDataRole, setHeaderData(), QHeaderView
*/
QVariant QGenericItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return impl->callConst<QVariant>(QGenericItemModelImplBase::HeaderData,
                                     section, orientation, role);
}

/*!
    \reimp

    Returns the data stored under the given \a role for the value in the
    range referred to by the \a index.

    If the item type for that index is an associative container that maps from
    either \c{int}, Qt::ItemDataRole, or QString to a QVariant, then the role
    data is looked up in that container and returned.

    Otherwise, the implementation returns a QVariant constructed from the item
    via \c{QVariant::fromValue()} for \c{Qt::DisplayRole} or \c{Qt::EditRole}.
    For other roles, the implementation returns an \b invalid
    (default-constructed) QVariant.

    \sa Qt::ItemDataRole, setData(), headerData()
*/
QVariant QGenericItemModel::data(const QModelIndex &index, int role) const
{
    return impl->callConst<QVariant>(QGenericItemModelImplBase::Data, index, role);
}

/*!
    \reimp

    Sets the \a role data for the item at \a index to \a data.

    If the item type for that \a index is an associative container that maps
    from either \c{int}, Qt::ItemDataRole, or QString to a QVariant, then
    \a data is stored in that container for the key specified by \a role.

    Otherwise, this implementation assigns the value in \a data to the item at
    the \a index in the range for \c{Qt::DisplayRole} and \c{Qt::EditRole},
    and returns \c{true}. For other roles, the implementation returns
    \c{false}.

//! [read-only-setData]
    For models operating on a read-only range, or on a read-only column in
    a row type that implements \l{the C++ tuple protocol}, this implementation
    returns \c{false} immediately.
//! [read-only-setData]
*/
bool QGenericItemModel::setData(const QModelIndex &index, const QVariant &data, int role)
{
    return impl->call<bool>(QGenericItemModelImplBase::SetData, index, data, role);
}

/*!
    \reimp

    Returns a map with values for all predefined roles in the model for the
    item at the given \a index.

    If the item type for that \a index is an associative container that maps
    from either \c{int}, Qt::ItemDataRole, or QString to a QVariant, then the
    data from that container is returned.

    If the item type is a gadget or QObject subclass, then the values of those
    properties that match a \l{roleNames()}{role name} are returned.

    If the item is not an associative container, gadget, or QObject subclass,
    then this calls the base class implementation.

    \sa setItemData(), Qt::ItemDataRole, data()
*/
QMap<int, QVariant> QGenericItemModel::itemData(const QModelIndex &index) const
{
    return impl->callConst<QMap<int, QVariant>>(QGenericItemModelImplBase::ItemData, index);
}

/*!
    \reimp

    If the item type for that \a index is an associative container that maps
    from either \c{int} or Qt::ItemDataRole to a QVariant, then the entries in
    \a data are stored in that container. If the associative container maps from
    QString to QVariant, then only those values in \a data are stored for which
    there is a mapping in the \l{roleNames()}{role names} table.

    If the item type is a gadget or QObject subclass, then those properties that
    match a \l{roleNames()}{role name} are set to the corresponding value in
    \a data.

    Roles for which there is no entry in \a data are not modified.

    For item types that can be copied, this implementation is transactional,
    and returns true if all the entries from \a data could be stored. If any
    entry could not be updated, then the original container is not modified at
    all, and the function returns false.

    If the item is not an associative container, gadget, or QObject subclass,
    then this calls the base class implementation, which calls setData() for
    each entry in \a data.

    \sa itemData(), setData(), Qt::ItemDataRole
*/
bool QGenericItemModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &data)
{
    return impl->call<bool>(QGenericItemModelImplBase::SetItemData, index, data);
}

/*!
    \reimp

    Replaces the value stored in the range at \a index with a default-
    constructed value.

    \include qgenericitemmodel.cpp read-only-setData
*/
bool QGenericItemModel::clearItemData(const QModelIndex &index)
{
    return impl->call<bool>(QGenericItemModelImplBase::ClearItemData, index);
}

/*
//! [column-change-requirement]
    \note A dynamically sized row type needs to provide a \c{\1} member function.

    For models operating on a read-only range, or on a range with a
    statically sized row type (such as a tuple, array, or struct), this
    implementation does nothing and returns \c{false} immediately. This is
    always the case for tree models.
//! [column-change-requirement]
*/

/*!
    \reimp

    Inserts \a count empty columns before the item at \a column in all rows
    of the range at \a parent. Returns \c{true} if successful; otherwise
    returns \c{false}.

    \include qgenericitemmodel.cpp {column-change-requirement} {insert(const_iterator, size_t, value_type)}
*/
bool QGenericItemModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    return impl->call<bool>(QGenericItemModelImplBase::InsertColumns, column, count, parent);
}

/*!
    \reimp

    Removes \a count columns from the item at \a column on in all rows of the
    range at \a parent. Returns \c{true} if successful, otherwise returns
    \c{false}.

    \include qgenericitemmodel.cpp {column-change-requirement} {erase(const_iterator, size_t)}
*/
bool QGenericItemModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    return impl->call<bool>(QGenericItemModelImplBase::RemoveColumns, column, count, parent);
}

/*
//! [row-change-requirement]
    \note The range needs to be dynamically sized and provide a \c{\1}
    member function.

    For models operating on a read-only or statically-sized range (such as
    an array), this implementation does nothing and returns \c{false}
    immediately.
//! [row-change-requirement]
*/

/*!
    \reimp

    Inserts \a count empty rows before the given \a row into the range at
    \a parent. Returns \c{true} if successful; otherwise returns \c{false}.

    \include qgenericitemmodel.cpp {row-change-requirement} {insert(const_iterator, size_t, value_type)}

    \note For ranges with a dynamically sized column type, the column needs
    to provide a \c{resize(size_t)} member function.
*/
bool QGenericItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return impl->call<bool>(QGenericItemModelImplBase::InsertRows, row, count, parent);
}

/*!
    \reimp

    Removes \a count rows from the range at \a parent, starting with the
    given \a row. Returns \c{true} if successful, otherwise returns \c{false}.

    \include qgenericitemmodel.cpp {row-change-requirement} {erase(const_iterator, size_t)}
*/
bool QGenericItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return impl->call<bool>(QGenericItemModelImplBase::RemoveRows, row, count, parent);
}

QT_END_NAMESPACE

#include "moc_qgenericitemmodel.cpp"
