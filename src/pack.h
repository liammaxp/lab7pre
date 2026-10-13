#ifndef PACK_H
#define PACK_H

namespace pack_detail {
  template <typename... Args>
  constexpr int packSize = static_cast<int>(sizeof...(Args));
  template <int QueryIndex, typename... Args>
  struct _packIndexToType {
    static_assert(QueryIndex >= 0 && QueryIndex < static_cast<int>(sizeof...(Args)),"Index out of range");
  };
  template <int QueryIndex, typename First, typename... Rest>
  struct _packIndexToType<QueryIndex, First, Rest...> {
    using type = typename _packIndexToType<QueryIndex - 1, Rest...>::type;
  };
  template <typename First, typename... Rest>
  struct _packIndexToType<0, First, Rest...> {
    using type = First;
  };
  template <int QueryIndex, typename... Args>
  using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;
  template <int Accumulated, typename QueryType, typename... Args>
  constexpr int _packTypeToIndex = -1;
  template <int Accumulated, typename QueryType, typename First, typename... Rest>
  constexpr int _packTypeToIndex<Accumulated, QueryType, First, Rest...> = _packTypeToIndex<Accumulated + 1, QueryType, Rest...>;
  template <int Accumulated, typename QueryType, typename... Rest>
  constexpr int _packTypeToIndex<Accumulated, QueryType, QueryType, Rest...> = Accumulated;
  template <typename QueryType, typename... Args>
  constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;
}
template <typename... Args>
struct Pack {
  static constexpr int size = pack_detail::packSize<Args...>;
  template <int QueryIndex>
  using type = pack_detail::packIndexToType<QueryIndex, Args...>;
  template <typename QueryType>
  static constexpr int index = pack_detail::packTypeToIndex<QueryType, Args...>;
};

#endif  // PACK_H
