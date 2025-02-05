// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtCore/qgenericitemmodel.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtreeview.h>

#include <array>
#include <vector>

void array()
{
    QListView listView;

//! [array]
std::array<int, 5> numbers = {1, 2, 3, 4, 5};
QGenericItemModel model(numbers);
listView.setModel(&model);
//! [array]
}

void const_array()
{
//! [const_array]
const std::array<int, 5> numbers = {1, 2, 3, 4, 5};
//! [const_array]
QGenericItemModel model(numbers);
}

void const_values()
{
//! [const_values]
std::array<const int, 5> numbers = {1, 2, 3, 4, 5};
//! [const_values]
QGenericItemModel model(numbers);
}

void list_of_int()
{
//! [list_of_int]
QList<int> numbers = {1, 2, 3, 4, 5};
QGenericItemModel model(numbers); // columnCount() == 1
QListView listView;
listView.setModel(&model);
//! [list_of_int]
}

void grid_of_numbers()
{
//! [grid_of_numbers]
std::vector<std::vector<int>> gridOfNumbers = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10},
    {11, 12, 13, 14, 15},
};
QGenericItemModel model(&gridOfNumbers); // columnCount() == 5
QTableView tableView;
tableView.setModel(&model);
//! [grid_of_numbers]
}

void pair_int_QString()
{
//! [pair_int_QString]
using TableRow = std::tuple<int, QString>;
QList<TableRow> numberNames = {
    {1, "one"},
    {2, "two"},
    {3, "three"}
};
QGenericItemModel model(&numberNames); // columnCount() == 2
QTableView tableView;
tableView.setModel(&model);
//! [pair_int_QString]
}

#if defined(__cpp_concepts) && defined(__cpp_lib_forward_like)
//! [tuple_protocol]
struct Book
{
    QString title;
    QString author;
    QString summary;
    int rating = 0;

    template <size_t I, typename T>
        requires ((I <= 3) && std::is_same_v<std::remove_cvref_t<T>, Book>)
    friend inline decltype(auto) get(T &&book)
    {
        if constexpr (I == 0)
            return std::as_const(book.title);
        else if constexpr (I == 1)
            return std::as_const(book.author);
        else if constexpr (I == 2)
            return std::forward_like<T>(book.summary);
        else if constexpr (I == 3)
            return std::forward_like<T>(book.rating);
    }
};

namespace std {
    template <> struct tuple_size<Book> : std::integral_constant<size_t, 4> {};
    template <size_t I> struct tuple_element<I, Book>
    { using type = decltype(get<I>(std::declval<Book>())); };
}
//! [tuple_protocol]
#endif // __cpp_concepts && forward_like
