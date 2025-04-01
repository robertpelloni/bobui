// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtCore/qgenericitemmodel.h>

#ifndef QT_NO_WIDGETS

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

namespace gadget {
//! [gadget]
class Book
{
    Q_GADGET
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString author READ author)
    Q_PROPERTY(QString summary MEMBER m_summary)
    Q_PROPERTY(int rating READ rating WRITE setRating)
public:
    Book(const QString &title, const QString &author);

    // C++ rule of 0: destructor, as well as copy/move operations
    // provided by the compiler.

    // read-only properties
    QString title() const { return m_title; }
    QString author() const { return m_author; }

    // read/writable property with input validation
    int rating() const { return m_rating; }
    void setRating(int rating)
    {
        m_rating = qBound(0, rating, 5);
    }

private:
    QString m_title;
    QString m_author;
    QString m_summary;
    int m_rating = 0;
};
//! [gadget]
} // namespace gadget

void color_map()
{
//! [color_map]
using ColorEntry = QMap<Qt::ItemDataRole, QVariant>;

const QStringList colorNames = QColor::colorNames();
QList<ColorEntry> colors;
colors.reserve(colorNames.size());
for (const QString &name : colorNames) {
    const QColor color = QColor::fromString(name);
    colors << ColorEntry{{Qt::DisplayRole, name},
                         {Qt::DecorationRole, color},
                         {Qt::ToolTipRole, color.name()}};
}
QGenericItemModel colorModel(colors);
QListView list;
list.setModel(&colorModel);
//! [color_map]
}

namespace multirole_gadget {
//! [color_gadget_0]
class ColorEntry
{
    Q_GADGET
    Q_PROPERTY(QString display MEMBER m_colorName)
    Q_PROPERTY(QColor decoration READ decoration)
    Q_PROPERTY(QString toolTip READ toolTip)
public:
    ColorEntry(const QString &color = {})
        : m_colorName(color)
    {}

    QColor decoration() const
    {
        return QColor::fromString(m_colorName);
    }
    QString toolTip() const
    {
        return QColor::fromString(m_colorName).name();
    }

private:
    QString m_colorName;
};
//! [color_gadget_0]

void color_list() {
//! [color_gadget_1]
const QStringList colorNames = QColor::colorNames();
QList<QGenericItemModel::SingleColumn<ColorEntry>> colors;
colors.reserve(colorNames.size());
for (const QString &name : colorNames)
    colors << ColorEntry{name};

QGenericItemModel colorModel(colors);
QListView list;
list.setModel(&colorModel);
//! [color_gadget_1]
}
} // namespace multirole_gadget

#endif // QT_NO_WIDGETS
