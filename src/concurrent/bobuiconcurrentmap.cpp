// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

/*!
    \namespace BobUIConcurrent
    \inmodule BobUIConcurrent
    \since 4.4
    \brief The BobUIConcurrent namespace provides high-level APIs that make it
    possible to write multi-threaded programs without using low-level
    threading primitives.

    See the \l {BobUI Concurrent} module documentation for an overview of available
    functions, or see below for detailed information on each function.

    \section1 Optimize includes

    If you include the \c <BobUIConcurrent> header, the entire BobUI Concurrent
    module with the entire Qt Core module will be included, which may increase
    compilation times and binary sizes. To use individual functions from the
    BobUIConcurrent namespace, you can include more specific headers.

    The table below lists the functions in the BobUIConcurrent namespace and
    their corresponding headers:

    \table
    \header
        \li Function
        \li Header
    \row
        \li \l {BobUIConcurrent::run}{BobUIConcurrent::run()}
        \li \c <BobUIConcurrentRun>
    \row
        \li \l {BobUIConcurrent::task}{BobUIConcurrent::task()}
        \li \c <BobUIConcurrentTask>
    \row
        \li \l {BobUIConcurrent::filter}{BobUIConcurrent::filter()},
            \l {BobUIConcurrent::filtered}{BobUIConcurrent::filtered()},
            \l {BobUIConcurrent::filteredReduced}{BobUIConcurrent::filteredReduced()}
        \li \c <BobUIConcurrentFilter>
    \row
        \li \l {BobUIConcurrent::map}{BobUIConcurrent::map()},
            \l {BobUIConcurrent::mapped}{BobUIConcurrent::mapped()},
            \l {BobUIConcurrent::mappedReduced}{BobUIConcurrent::mappedReduced()}
        \li \c <BobUIConcurrentMap>
    \endtable

    \inheaderfile BobUIConcurrent
    \ingroup thread
*/

/*!
  \enum BobUIConcurrent::ReduceQueueLimits
  \internal
 */

/*!
  \class BobUIConcurrent::ReduceKernel
  \inmodule BobUIConcurrent
  \internal
*/

/*!
  \class BobUIConcurrent::SequenceHolder2
  \inmodule BobUIConcurrent
  \internal
*/

/*!
  \class BobUIConcurrent::MapKernel
  \inmodule BobUIConcurrent
  \internal
*/

/*!
  \class BobUIConcurrent::MappedReducedKernel
  \inmodule BobUIConcurrent
  \internal
*/

/*!
  \class BobUIConcurrent::MappedEachKernel
  \inmodule BobUIConcurrent
  \internal
*/

/*!
  \class BobUIConcurrent::SequenceHolder1
  \inmodule BobUIConcurrent
  \internal
*/

/*!
  \fn [qtconcurrentmapkernel-1] ThreadEngineStarter<void> BobUIConcurrent::startMap(Iterator begin, Iterator end, Functor &&functor)
  \internal
*/

/*!
  \fn [qtconcurrentmapkernel-2] ThreadEngineStarter<T> BobUIConcurrent::startMapped(Iterator begin, Iterator end, Functor &&functor)
  \internal
*/

/*!
  \fn [qtconcurrentmapkernel-3] ThreadEngineStarter<T> BobUIConcurrent::startMapped(Sequence &&sequence, Functor &&functor)
  \internal
*/

/*!
  \fn [qtconcurrentmapkernel-4] ThreadEngineStarter<ResultType> BobUIConcurrent::startMappedReduced(Sequence &&sequence, MapFunctor &&mapFunctor, ReduceFunctor &&reduceFunctor, ReduceOptions options)
  \internal
*/

/*!
  \fn [qtconcurrentmapkernel-5] ThreadEngineStarter<ResultType> BobUIConcurrent::startMappedReduced(Iterator begin, Iterator end, MapFunctor &&mapFunctor, ReduceFunctor &&reduceFunctor, ReduceOptions options)
  \internal
*/

/*!
  \fn [qtconcurrentmapkernel-6] ThreadEngineStarter<ResultType> BobUIConcurrent::startMappedReduced(Sequence &&sequence, MapFunctor &&mapFunctor, ReduceFunctor &&reduceFunctor, ResultType &&initialValue, ReduceOptions options)
  \internal
*/

/*!
  \fn [qtconcurrentmapkernel-7] ThreadEngineStarter<ResultType> BobUIConcurrent::startMappedReduced(Iterator begin, Iterator end, MapFunctor &&mapFunctor, ReduceFunctor &&reduceFunctor, ResultType &&initialValue, ReduceOptions options)
  \internal
*/

/*!
    \enum BobUIConcurrent::ReduceOption
    This enum specifies the order of which results from the map or filter
    function are passed to the reduce function.

    \value UnorderedReduce Reduction is done in an arbitrary order.
    \value OrderedReduce Reduction is done in the order of the
    original sequence.
    \value SequentialReduce Reduction is done sequentially: only one
    thread will enter the reduce function at a time. (Parallel reduction
    might be supported in a future version of BobUI Concurrent.)
*/

/*!
    \page qtconcurrentmap.html
    \title Concurrent Map and Map-Reduce
    \brief Transforming values from a sequence and combining them, all in parallel.
    \ingroup thread

    The BobUIConcurrent::map(), BobUIConcurrent::mapped() and
    BobUIConcurrent::mappedReduced() functions run computations in parallel on
    the items in a sequence such as a QList. BobUIConcurrent::map()
    modifies a sequence in-place, BobUIConcurrent::mapped() returns a new
    sequence containing the modified content, and BobUIConcurrent::mappedReduced()
    returns a single result.

    These functions are part of the \l {BobUI Concurrent} framework.

    Each of the above functions has a blocking variant that returns
    the final result instead of a QFuture. You use them in the same
    way as the asynchronous variants.

    \snippet code/src_concurrent_qtconcurrentmap.cpp 7

    Note that the result types above are not QFuture objects, but real result
    types (in this case, QList<QImage> and QImage).

    \section1 Optimize includes

    If you include the \c <BobUIConcurrent> header, the entire BobUI Concurrent
    module with the entire Qt Core module will be included, which may increase
    compilation times and binary sizes. To use the
    \l {BobUIConcurrent::map}{BobUIConcurrent::map()},
    \l {BobUIConcurrent::mapped}{BobUIConcurrent::mapped()}, and
    \l {BobUIConcurrent::mappedReduced}{BobUIConcurrent::mappedReduced()} functions,
    you can include a more specific header:

    \code
    #include <BobUIConcurrentMap>
    \endcode

    \section1 Concurrent Map

    BobUIConcurrent::mapped() takes an input sequence and a map function. This map
    function is then called for each item in the sequence, and a new sequence
    containing the return values from the map function is returned.

    The map function must be of the form:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 0

    T and U can be any type (and they can even be the same type), but T must
    match the type stored in the sequence. The function returns the modified
    or \e mapped content.

    This example shows how to apply a scale function to all the items
    in a sequence:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 1

    The results of the map are made available through QFuture.  See the
    QFuture and QFutureWatcher documentation for more information on how to
    use QFuture in your applications.

    If you want to modify a sequence in-place, use BobUIConcurrent::map(). The
    map function must then be of the form:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 2

    Note that the return value and return type of the map function are not
    used.

    Using BobUIConcurrent::map() is similar to using BobUIConcurrent::mapped():

    \snippet code/src_concurrent_qtconcurrentmap.cpp 3

    Since the sequence is modified in place, BobUIConcurrent::map() does not
    return any results via QFuture. However, you can still use QFuture and
    QFutureWatcher to monitor the status of the map.

    \section2 Concurrent Mapped and Continuations

    The result of BobUIConcurrent::mapped() call is a QFuture that contains
    multiple results. When attaching a \c {.then()} continuation to such
    QFuture, make sure to use a continuation that takes QFuture as a parameter,
    otherwise only the first result will be processed:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 18

    In this example \c {badFuture} will only print a single result, while
    \c {goodFuture} will print all results.

    \section1 Concurrent Map-Reduce

    BobUIConcurrent::mappedReduced() is similar to BobUIConcurrent::mapped(), but
    instead of returning a sequence with the new results, the results are
    combined into a single value using a reduce function.

    The reduce function must be of the form:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 4

    T is the type of the final result, U is the return type of the map
    function. Note that the return value and return type of the reduce
    function are not used.

    Call BobUIConcurrent::mappedReduced() like this:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 5

    The reduce function will be called once for each result returned by the map
    function, and should merge the \e{intermediate} into the \e{result}
    variable.  BobUIConcurrent::mappedReduced() guarantees that only one thread
    will call reduce at a time, so using a mutex to lock the result variable
    is not necessary. The BobUIConcurrent::ReduceOptions enum provides a way to
    control the order in which the reduction is done. If
    BobUIConcurrent::UnorderedReduce is used (the default), the order is
    undefined, while BobUIConcurrent::OrderedReduce ensures that the reduction
    is done in the order of the original sequence.

    \section1 Additional API Features

    \section2 Using Iterators instead of Sequence

    Each of the above functions has a variant that takes an iterator range
    instead of a sequence. You use them in the same way as the sequence
    variants:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 6

    \section2 Blocking Variants

    Each of the above functions has a blocking variant that returns
    the final result instead of a QFuture. You use them in the same
    way as the asynchronous variants.

    \snippet code/src_concurrent_qtconcurrentmap.cpp 7

    Note that the result types above are not QFuture objects, but real result
    types (in this case, QList<QImage> and QImage).

    \section2 Using Member Functions

    BobUIConcurrent::map(), BobUIConcurrent::mapped(), and
    BobUIConcurrent::mappedReduced() accept pointers to member functions.
    The member function class type must match the type stored in the sequence:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 8

    Note the use of qOverload. It is needed to resolve the ambiguity for the
    methods, that have multiple overloads.

    Also note that when using BobUIConcurrent::mappedReduced(), you can mix the use of
    normal and member functions freely:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 9

    \section2 Using Function Objects

    BobUIConcurrent::map(), BobUIConcurrent::mapped(), and
    BobUIConcurrent::mappedReduced() accept function objects
    for the map function. These function objects can be used to
    add state to a function call:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 14

    Function objects are also supported for the reduce function:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 11

    \section2 Using Lambda Expressions

    BobUIConcurrent::map(), BobUIConcurrent::mapped(), and
    BobUIConcurrent::mappedReduced() accept lambda expressions for the map and
    reduce function:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 15

    When using BobUIConcurrent::mappedReduced() or
    BobUIConcurrent::blockingMappedReduced(), you can mix the use of normal
    functions, member functions and lambda expressions freely.

    \snippet code/src_concurrent_qtconcurrentmap.cpp 16

    You can also pass a lambda as a reduce object:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 17

    \section2 Wrapping Functions that Take Multiple Arguments

    If you want to use a map function that takes more than one argument you can
    use a lambda function or \c std::bind() to transform it onto a function that
    takes one argument.

    As an example, we'll use QImage::scaledToWidth():

    \snippet code/src_concurrent_qtconcurrentmap.cpp 10

    scaledToWidth takes three arguments (including the "this" pointer) and
    can't be used with BobUIConcurrent::mapped() directly, because
    BobUIConcurrent::mapped() expects a function that takes one argument. To use
    QImage::scaledToWidth() with BobUIConcurrent::mapped() we have to provide a
    value for the \e{width} and the \e{transformation mode}:

    \snippet code/src_concurrent_qtconcurrentmap.cpp 13
*/

/*!
    \fn template <typename Sequence, typename MapFunctor> QFuture<void> BobUIConcurrent::map(QThreadPool *pool, Sequence &&sequence, MapFunctor &&function)

    Calls \a function once for each item in \a sequence.
    All calls to \a function are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The \a function takes a reference to the item, so that any modifications done to the item
    will appear in \a sequence.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Sequence, typename MapFunctor> QFuture<void> BobUIConcurrent::map(Sequence &&sequence, MapFunctor &&function)

    Calls \a function once for each item in \a sequence. The \a function takes
    a reference to the item, so that any modifications done to the item
    will appear in \a sequence.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Iterator, typename MapFunctor> QFuture<void> BobUIConcurrent::map(QThreadPool *pool, Iterator begin, Iterator end, MapFunctor &&function)

    Calls \a function once for each item from \a begin to \a end.
    All calls to \a function are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The \a function takes a reference to the item, so that any modifications
    done to the item will appear in the sequence which the iterators belong to.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Iterator, typename MapFunctor> QFuture<void> BobUIConcurrent::map(Iterator begin, Iterator end, MapFunctor &&function)

    Calls \a function once for each item from \a begin to \a end. The
    \a function takes a reference to the item, so that any modifications
    done to the item will appear in the sequence which the iterators belong to.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Sequence, typename MapFunctor> QFuture<QtPrivate::MapResultType<Sequence, MapFunctor>> BobUIConcurrent::mapped(QThreadPool *pool, Sequence &&sequence, MapFunctor &&function)

    Calls \a function once for each item in \a sequence and returns a future
    with each mapped item as a result. All calls to \a function are invoked from the
    threads taken from the BOBUIhreadPool \a pool. You can use QFuture::const_iterator or
    QFutureIterator to iterate through the results.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Sequence, typename MapFunctor> QFuture<QtPrivate::MapResultType<Sequence, MapFunctor>> BobUIConcurrent::mapped(Sequence &&sequence, MapFunctor &&function)

    Calls \a function once for each item in \a sequence and returns a future
    with each mapped item as a result. You can use QFuture::const_iterator or
    QFutureIterator to iterate through the results.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Iterator, typename MapFunctor> QFuture<QtPrivate::MapResultType<Iterator, MapFunctor>> BobUIConcurrent::mapped(QThreadPool *pool, Iterator begin, Iterator end, MapFunctor &&function)

    Calls \a function once for each item from \a begin to \a end and returns a
    future with each mapped item as a result. All calls to \a function are invoked from the
    threads taken from the BOBUIhreadPool \a pool. You can use
    QFuture::const_iterator or QFutureIterator to iterate through the results.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Iterator, typename MapFunctor> QFuture<QtPrivate::MapResultType<Iterator, MapFunctor>> BobUIConcurrent::mapped(Iterator begin, Iterator end, MapFunctor &&function)

    Calls \a function once for each item from \a begin to \a end and returns a
    future with each mapped item as a result. You can use
    QFuture::const_iterator or QFutureIterator to iterate through the results.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Sequence, typename MapFunctor, typename ReduceFunctor> QFuture<ResultType> BobUIConcurrent::mappedReduced(QThreadPool *pool, Sequence &&sequence, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item in \a sequence.
    All calls to \a mapFunction are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The return value of each \a mapFunction is passed to \a reduceFunction.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. The order in which \a reduceFunction is
    called is determined by \a reduceOptions.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Sequence, typename MapFunctor, typename ReduceFunctor> QFuture<ResultType> BobUIConcurrent::mappedReduced(Sequence &&sequence, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item in \a sequence. The return value of
    each \a mapFunction is passed to \a reduceFunction.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. The order in which \a reduceFunction is
    called is determined by \a reduceOptions.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Sequence, typename MapFunctor, typename ReduceFunctor, typename InitialValueType> QFuture<ResultType> BobUIConcurrent::mappedReduced(QThreadPool *pool, Sequence &&sequence, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, InitialValueType &&initialValue, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item in \a sequence.
    All calls to \a mapFunction are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The return value of each \a mapFunction is passed to \a reduceFunction.
    The result value is initialized to \a initialValue when the function is
    called, and the first call to \a reduceFunction will operate on
    this value.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. The order in which \a reduceFunction is
    called is determined by \a reduceOptions.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Sequence, typename MapFunctor, typename ReduceFunctor, typename InitialValueType> QFuture<ResultType> BobUIConcurrent::mappedReduced(Sequence &&sequence, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, InitialValueType &&initialValue, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item in \a sequence. The return value of
    each \a mapFunction is passed to \a reduceFunction.
    The result value is initialized to \a initialValue when the function is
    called, and the first call to \a reduceFunction will operate on
    this value.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. The order in which \a reduceFunction is
    called is determined by \a reduceOptions.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Iterator, typename MapFunctor, typename ReduceFunctor> QFuture<ResultType> BobUIConcurrent::mappedReduced(QThreadPool *pool, Iterator begin, Iterator end, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item from \a begin to \a end.
    All calls to \a mapFunction are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The return value of each \a mapFunction is passed to \a reduceFunction.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. By default, the order in which
    \a reduceFunction is called is undefined.

    \note BobUIConcurrent::OrderedReduce results in the ordered reduction.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Iterator, typename MapFunctor, typename ReduceFunctor> QFuture<ResultType> BobUIConcurrent::mappedReduced(Iterator begin, Iterator end, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item from \a begin to \a end. The return
    value of each \a mapFunction is passed to \a reduceFunction.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. By default, the order in which
    \a reduceFunction is called is undefined.

    \note BobUIConcurrent::OrderedReduce results in the ordered reduction.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Iterator, typename MapFunctor, typename ReduceFunctor, typename InitialValueType> QFuture<ResultType> BobUIConcurrent::mappedReduced(QThreadPool *pool, Iterator begin, Iterator end, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, InitialValueType &&initialValue, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item from \a begin to \a end.
    All calls to \a mapFunction are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The return value of each \a mapFunction is passed to \a reduceFunction.
    The result value is initialized to \a initialValue when the function is
    called, and the first call to \a reduceFunction will operate on
    this value.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. By default, the order in which
    \a reduceFunction is called is undefined.

    \note BobUIConcurrent::OrderedReduce results in the ordered reduction.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Iterator, typename MapFunctor, typename ReduceFunctor, typename InitialValueType> QFuture<ResultType> BobUIConcurrent::mappedReduced(Iterator begin, Iterator end, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, InitialValueType &&initialValue, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item from \a begin to \a end. The return
    value of each \a mapFunction is passed to \a reduceFunction.
    The result value is initialized to \a initialValue when the function is
    called, and the first call to \a reduceFunction will operate on
    this value.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. By default, the order in which
    \a reduceFunction is called is undefined.

    \note BobUIConcurrent::OrderedReduce results in the ordered reduction.

    \sa {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Sequence, typename MapFunctor> void BobUIConcurrent::blockingMap(QThreadPool *pool, Sequence &&sequence, MapFunctor function)

    Calls \a function once for each item in \a sequence.
    All calls to \a function are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The \a function takes a reference to the item, so that any modifications done to the item
    will appear in \a sequence.

    \note This function will block until all items in the sequence have been processed.

    \sa map(), {Concurrent Map and Map-Reduce}
*/

/*!
  \fn template <typename Sequence, typename MapFunctor> void BobUIConcurrent::blockingMap(Sequence &&sequence, MapFunctor &&function)

  Calls \a function once for each item in \a sequence. The \a function takes
  a reference to the item, so that any modifications done to the item
  will appear in \a sequence.

  \note This function will block until all items in the sequence have been processed.

  \sa map(), {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Iterator, typename MapFunctor> void BobUIConcurrent::blockingMap(QThreadPool *pool, Iterator begin, Iterator end, MapFunctor &&function)

    Calls \a function once for each item from \a begin to \a end.
    All calls to \a function are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The \a function takes a reference to the item, so that any modifications
    done to the item will appear in the sequence which the iterators belong to.

    \note This function will block until the iterator reaches the end of the
    sequence being processed.

    \sa map(), {Concurrent Map and Map-Reduce}
*/

/*!
  \fn template <typename Iterator, typename MapFunctor> void BobUIConcurrent::blockingMap(Iterator begin, Iterator end, MapFunctor &&function)

  Calls \a function once for each item from \a begin to \a end. The
  \a function takes a reference to the item, so that any modifications
  done to the item will appear in the sequence which the iterators belong to.

  \note This function will block until the iterator reaches the end of the
  sequence being processed.

  \sa map(), {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename OutputSequence, typename InputSequence, typename MapFunctor> OutputSequence BobUIConcurrent::blockingMapped(QThreadPool *pool, InputSequence &&sequence, MapFunctor &&function)

    Calls \a function once for each item in \a sequence and returns an OutputSequence containing
    the results. All calls to \a function are invoked from the threads taken from the BOBUIhreadPool
    \a pool. The type of the results will match the type returned by the MapFunctor.

    \note This function will block until all items in the sequence have been processed.

    \sa mapped(), {Concurrent Map and Map-Reduce}
*/

/*!
  \fn template <typename OutputSequence, typename InputSequence, typename MapFunctor> OutputSequence BobUIConcurrent::blockingMapped(InputSequence &&sequence, MapFunctor &&function)

  Calls \a function once for each item in \a sequence and returns an OutputSequence containing
  the results. The type of the results will match the type returned by the MapFunctor.

  \note This function will block until all items in the sequence have been processed.

  \sa mapped(), {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename Sequence, typename Iterator, typename MapFunctor> Sequence BobUIConcurrent::blockingMapped(QThreadPool *pool, Iterator begin, Iterator end, MapFunctor &&function)

    Calls \a function once for each item from \a begin to \a end and returns a
    container with the results. All calls to \a function are invoked from the threads
    taken from the BOBUIhreadPool \a pool. You can specify the type of container as the a template
    argument, like this:

    \code
        QList<int> ints = BobUIConcurrent::blockingMapped<QList<int> >(beginIterator, endIterator, fn);
    \endcode

    \note This function will block until the iterator reaches the end of the
    sequence being processed.

    \sa mapped(), {Concurrent Map and Map-Reduce}
*/

/*!
  \fn template <typename Sequence, typename Iterator, typename MapFunctor> Sequence BobUIConcurrent::blockingMapped(Iterator begin, Iterator end, MapFunctor &&function)

  Calls \a function once for each item from \a begin to \a end and returns a
  container with the results. You can specify the type of container as the a template
  argument, like this:

  \code
     QList<int> ints = BobUIConcurrent::blockingMapped<QList<int> >(beginIterator, endIterator, fn);
  \endcode

  \note This function will block until the iterator reaches the end of the
  sequence being processed.

  \sa mapped(), {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Sequence, typename MapFunctor, typename ReduceFunctor> ResultType BobUIConcurrent::blockingMappedReduced(QThreadPool *pool, Sequence &&sequence, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item in \a sequence.
    All calls to \a mapFunction are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The return value of each \a mapFunction is passed to \a reduceFunction.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. The order in which \a reduceFunction is
    called is determined by \a reduceOptions.

    \note This function will block until all items in the sequence have been processed.

    \sa mapped(), {Concurrent Map and Map-Reduce}
*/

/*!
  \fn template <typename ResultType, typename Sequence, typename MapFunctor, typename ReduceFunctor> ResultType BobUIConcurrent::blockingMappedReduced(Sequence &&sequence, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, BobUIConcurrent::ReduceOptions reduceOptions)

  Calls \a mapFunction once for each item in \a sequence. The return value of
  each \a mapFunction is passed to \a reduceFunction.

  Note that while \a mapFunction is called concurrently, only one thread at a
  time will call \a reduceFunction. The order in which \a reduceFunction is
  called is determined by \a reduceOptions.

  \note This function will block until all items in the sequence have been processed.

  \sa mapped(), {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Sequence, typename MapFunctor, typename ReduceFunctor, typename InitialValueType> ResultType BobUIConcurrent::blockingMappedReduced(QThreadPool *pool, Sequence &&sequence, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, InitialValueType &&initialValue, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item in \a sequence.
    All calls to \a mapFunction are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The return value of each \a mapFunction is passed to \a reduceFunction.
    The result value is initialized to \a initialValue when the function is
    called, and the first call to \a reduceFunction will operate on
    this value.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. The order in which \a reduceFunction is
    called is determined by \a reduceOptions.

    \note This function will block until all items in the sequence have been processed.

    \sa mapped(), {Concurrent Map and Map-Reduce}
*/

/*!
  \fn template <typename ResultType, typename Sequence, typename MapFunctor, typename ReduceFunctor, typename InitialValueType> ResultType BobUIConcurrent::blockingMappedReduced(Sequence &&sequence, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, InitialValueType &&initialValue, BobUIConcurrent::ReduceOptions reduceOptions)

  Calls \a mapFunction once for each item in \a sequence. The return value of
  each \a mapFunction is passed to \a reduceFunction.
  The result value is initialized to \a initialValue when the function is
  called, and the first call to \a reduceFunction will operate on
  this value.

  Note that while \a mapFunction is called concurrently, only one thread at a
  time will call \a reduceFunction. The order in which \a reduceFunction is
  called is determined by \a reduceOptions.

  \note This function will block until all items in the sequence have been processed.

  \sa mapped(), {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Iterator, typename MapFunctor, typename ReduceFunctor> ResultType BobUIConcurrent::blockingMappedReduced(QThreadPool *pool, Iterator begin, Iterator end, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item from \a begin to \a end.
    All calls to \a mapFunction are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The return value of each \a mapFunction is passed to \a reduceFunction.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. The order in which \a reduceFunction is
    called is undefined.

    \note This function will block until the iterator reaches the end of the
    sequence being processed.

    \sa blockingMappedReduced(), {Concurrent Map and Map-Reduce}
*/

/*!
  \fn template <typename ResultType, typename Iterator, typename MapFunctor, typename ReduceFunctor> ResultType BobUIConcurrent::blockingMappedReduced(Iterator begin, Iterator end, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, BobUIConcurrent::ReduceOptions reduceOptions)

  Calls \a mapFunction once for each item from \a begin to \a end. The return
  value of each \a mapFunction is passed to \a reduceFunction.

  Note that while \a mapFunction is called concurrently, only one thread at a
  time will call \a reduceFunction. The order in which \a reduceFunction is
  called is undefined.

  \note This function will block until the iterator reaches the end of the
  sequence being processed.

  \sa blockingMappedReduced(), {Concurrent Map and Map-Reduce}
*/

/*!
    \fn template <typename ResultType, typename Iterator, typename MapFunctor, typename ReduceFunctor, typename InitialValueType> ResultType BobUIConcurrent::blockingMappedReduced(QThreadPool *pool, Iterator begin, Iterator end, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, InitialValueType &&initialValue, BobUIConcurrent::ReduceOptions reduceOptions)

    Calls \a mapFunction once for each item from \a begin to \a end.
    All calls to \a mapFunction are invoked from the threads taken from the BOBUIhreadPool \a pool.
    The return value of each \a mapFunction is passed to \a reduceFunction.
    The result value is initialized to \a initialValue when the function is
    called, and the first call to \a reduceFunction will operate on
    this value.

    Note that while \a mapFunction is called concurrently, only one thread at a
    time will call \a reduceFunction. The order in which \a reduceFunction is
    called is undefined.

    \note This function will block until the iterator reaches the end of the
    sequence being processed.

    \sa blockingMappedReduced(), {Concurrent Map and Map-Reduce}
*/

/*!
  \fn template <typename ResultType, typename Iterator, typename MapFunctor, typename ReduceFunctor, typename InitialValueType> ResultType BobUIConcurrent::blockingMappedReduced(Iterator begin, Iterator end, MapFunctor &&mapFunction, ReduceFunctor &&reduceFunction, InitialValueType &&initialValue, BobUIConcurrent::ReduceOptions reduceOptions)

  Calls \a mapFunction once for each item from \a begin to \a end. The return
  value of each \a mapFunction is passed to \a reduceFunction.
  The result value is initialized to \a initialValue when the function is
  called, and the first call to \a reduceFunction will operate on
  this value.

  Note that while \a mapFunction is called concurrently, only one thread at a
  time will call \a reduceFunction. The order in which \a reduceFunction is
  called is undefined.

  \note This function will block until the iterator reaches the end of the
  sequence being processed.

  \sa blockingMappedReduced(), {Concurrent Map and Map-Reduce}
*/
