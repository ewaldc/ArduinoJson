// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>
#define offset(slot) (VariantSlotOffset)((VariantSlot *)this - slot)

namespace ARDUINOJSON_NAMESPACE {


inline VariantSlot* CollectionData::head() const {
	return (_head) ? (VariantSlot *)this - _head : nullptr;
}

inline VariantSlot* CollectionData::linkedHead() const {
	return _linkedHead;
}

inline VariantSlot* CollectionData::tail() const {
	return (VariantSlot *)this - _tail;
}

inline VariantSlot* CollectionData::addSlot(MemoryPool* pool) {
  VariantSlot* slot = pool->allocVariant();
  if (!slot)
    return 0;
	slot->clear();
	if (_head)
		this->tail()->setNext(slot);
	else
		_head = offset(slot);
	_tail = offset(slot);
	/*
	else { // find last slot
		VariantSlot* vs = _head;
		while (vs->next())
			vs = vs->next();
		//VariantSlot* vs = slot - 1;
		vs->setNext(slot);
	}
	/*
  if (_tail) {
    _tail->setNextNotNull(slot);
    _tail = slot;
  } else {
    _head = slot;
    _tail = slot;
  }
	*/
	 return slot;
}

inline VariantData* CollectionData::addElement(MemoryPool* pool) {
  return slotData(addSlot(pool));
}

template <typename TAdaptedString>
inline VariantData* CollectionData::addMember(TAdaptedString key,
                                              MemoryPool* pool) {
  VariantSlot* slot = addSlot(pool);
  if (!slotSetKey(slot, key, pool)) {
    removeSlot(slot);
    return 0;
  }
  return slot->data();
}

inline void CollectionData::clear() {
  _head = 0;
  _tail = 0;
}

template <typename TAdaptedString>
inline bool CollectionData::containsKey(const TAdaptedString& key) const {
  return getSlot(key) != 0;
}

inline bool CollectionData::copyFrom(const CollectionData& src,
                                     MemoryPool* pool) {
  clear();
  for (VariantSlot* s = src.head(); s; s = s->next()) {
    VariantData* var;
    if (s->key() != 0) {
      if (s->ownsKey())
        var = addMember(RamStringAdapter(pool->toCharPtr(s->key())), pool);
      else
        var = addMember(ConstRamStringAdapter(pool->toCharPtr(s->key())), pool);
    } else {
      var = addElement(pool);
    }
    if (!var)
      return false;
    if (!var->copyFrom(*s->data(), pool))
      return false;
  }
  return true;
}

inline bool CollectionData::equalsObject(MemoryPool &pool, const CollectionData& other, MemoryPool &otherPool) const {
  size_t count = 0;
  for (VariantSlot* slot = head(); slot; slot = slot->next()) {
    VariantData* v1 = slot->data();
    VariantData* v2 = other.getMember(adaptString(otherPool.toCharPtr(slot->key())));
    if (!variantEquals(v1, pool, v2, otherPool))
      return false;
    count++;
  }
  return count == other.size();
}

inline bool CollectionData::equalsArray(MemoryPool &pool, const CollectionData& other, MemoryPool &otherPool) const {
  VariantSlot* s1 = head();
  VariantSlot* s2 = other.head();
  for (;;) {
    if (s1 == s2)
      return true;
    if (!s1 || !s2)
      return false;
    if (!variantEquals(s1->data(), pool, s2->data(), otherPool))
      return false;
    s1 = s1->next();
    s2 = s2->next();
  }
}

template <typename TAdaptedString>
inline VariantSlot* CollectionData::getSlot(TAdaptedString key, MemoryPool *pool) const {
  VariantSlot* slot = head();
  while (slot) {
    if (key.equals(pool->toCharPtr(slot->key())))
      break;
    slot = slot->next();
  }
  return slot;
}

/*
template <typename TAdaptedString>
inline VariantSlot* CollectionData::getSlot(TAdaptedString key) const {
	VariantSlot* slot = _head;
	while (slot) {
		if (key.equals(slot->key()))
			break;
		slot = slot->next();
	}
	return slot;
}
*/

inline VariantSlot* CollectionData::getSlot(size_t index) const {
  return head()->next(index);
}

inline VariantSlot* CollectionData::getPreviousSlot(VariantSlot* target) const {
  VariantSlot* current = head();
  while (current) {
    VariantSlot* next = current->next();
    if (next == target)
      return current;
    current = next;
  }
  return 0;
}

template <typename TAdaptedString>
inline VariantData* CollectionData::getMember(TAdaptedString key, MemoryPool *pool) const {
	VariantSlot* slot = getSlot(key, pool);
	return slot ? slot->data() : 0;
}

/*
template <typename TAdaptedString>
inline VariantData* CollectionData::getMember(TAdaptedString key) const {
  VariantSlot* slot = getSlot(key);
  return slot ? slot->data() : 0;
}
*/

template <typename TAdaptedString>
inline VariantData* CollectionData::getOrAddMember(TAdaptedString key,
                                                   MemoryPool* pool) {
  // ignore null key
  if (key.isNull())
    return 0;

  // search a matching key
  VariantSlot* slot = getSlot(key);
  if (slot)
    return slot->data();

  return addMember(key, pool);
}

inline VariantData* CollectionData::getElement(size_t index) const {
  VariantSlot* slot = getSlot(index);
  return slot ? slot->data() : 0;
}

inline VariantData* CollectionData::getOrAddElement(size_t index,
                                                    MemoryPool* pool) {
  VariantSlot* slot = head();
  while (slot && index > 0) {
    slot = slot->next();
    index--;
  }
  if (!slot)
    index++;
  while (index > 0) {
    slot = addSlot(pool);
    index--;
  }
  return slotData(slot);
}

inline void CollectionData::removeSlot(VariantSlot* slot) {
  if (!slot)
    return;
  VariantSlot* prev = getPreviousSlot(slot);
  VariantSlot* next = slot->next();
  if (prev)
    prev->setNext(next);
  else
    _head = offset(next);
  //if (!next)
    //_tail = prev;
}

inline void CollectionData::removeElement(size_t index) {
  removeSlot(getSlot(index));
}

inline size_t CollectionData::memoryUsage(MemoryPool& pool) const {
  size_t total = 0;
  for (VariantSlot* s = head(); s; s = s->next()) {
    total += sizeof(VariantSlot) + s->data()->memoryUsage(pool);
    if (s->ownsKey())
      total += strlen(pool.toCharPtr(s->key())) + 1;
  }
  return total;
}

inline size_t CollectionData::nesting() const {
  size_t maxChildNesting = 0;
  for (VariantSlot* s = head(); s; s = s->next()) {
    size_t childNesting = s->data()->nesting();
    if (childNesting > maxChildNesting)
      maxChildNesting = childNesting;
  }
  return maxChildNesting + 1;
}

inline size_t CollectionData::size() const {
  return slotSize(head());
}

template <typename T>
inline void movePointer(T*& p, ptrdiff_t offset) {
  if (!p)
    return;
  p = reinterpret_cast<T*>(
      reinterpret_cast<void*>(reinterpret_cast<char*>(p) + offset));
  ARDUINOJSON_ASSERT(isAligned(p));
}

inline void CollectionData::movePointers(ptrdiff_t stringDistance,
                                         ptrdiff_t variantDistance) {
	VariantSlot * h = head();
  movePointer(h, variantDistance);
  //movePointer(_tail, variantDistance);
  for (VariantSlot* slot = h; slot; slot = slot->next())
    slot->movePointers(stringDistance, variantDistance);
}

}  // namespace ARDUINOJSON_NAMESPACE
