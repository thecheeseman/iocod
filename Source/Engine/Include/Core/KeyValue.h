// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_CORE_KEYVALUE_H
#define ENGINE_CORE_KEYVALUE_H

namespace iocod {

template <typename K, typename V>
struct KeyValue {
    const K key;
    V value;

    KeyValue(const KeyValue& kv) : key(kv.key), value(kv.value) {}
    Keyvalue(const K& key, const V& value) : key(key), value(value) {}

    void operator=(const KeyValue &) = delete;
};

template <typename K, typename V>
bool operator==(const KeyValue<K, V>& lhs, const KeyValue<K, V>& rhs)
{
    return lhs.key == rhs.key && lhs.value == rhs.value;
}

template <typename K, typename V>
bool operator!=(const KeyValue<K, V>& lhs, const KeyValue<K, V>& rhs)
{
    return !(lhs == rhs);
}

template <typename K, typename V>
struct KeyValueSort {
    bool operator()(const KeyValue<K, V>& lhs, const KeyValue<K, V>& rhs) const
    {
        return lhs.key < rhs.key;
    }
};

} // namespace iocod

#endif // ENGINE_CORE_KEYVALUE_H
