#ifndef DLLIST_H_
#define DLLIST_H_

/* template for a doubly linked list */

template <class T>
class DLList
{
	T *head;
	T *_next;
	bool iterate;
	unsigned int count;

public:
	T *Head() { return head; };
	T *Tail() { return (head) ? head->prev : (T *)0; };
	unsigned int Count() { return count; };

	// useful methods for iteration
	T *next() { return (_next) ? (_next = _next->next) : (T *)0; };
	T *reset()
	{
		iterate = false;
		return (_next = head);
	};
	bool atEnd()
	{
		if (!iterate && head)
		{
			iterate = true;
			return false;
		}
		return (!head) || (_next == head);
	};

	inline bool isEmpty() { return (head == 0); }

	void init(T *item)
	{
		head = item;
		item->next = item->prev = item;
		count = 1;
	};

	bool isSingleton()
	{
		return (!head || (head == head->next));
	};

	DLList()
	{
		head = (T *)0;
		count = 0;
	};
	DLList(T *item) { init(item); };

	inline void insertBefore(T *beforeItem, T *item)
	{
		item->next = beforeItem;
		item->prev = beforeItem->prev;
		beforeItem->prev->next = item;
		beforeItem->prev = item;
		if (beforeItem == head)
			head = item;
		count++;
	};

	inline void insertAfter(T *afterItem, T *item)
	{
		item->next = afterItem->next;
		item->prev = afterItem;
		afterItem->next->prev = item;
		afterItem->next = item;
		count++;
	};
	// remove item from the list
	inline void remove(T *item)
	{
		if (!isSingleton())
		{
			item->prev->next = item->next;
			item->next->prev = item->prev;
			if (item == head)
				head = item->next;
		}
		else
			head = (T *)0;
		count--;
	};

	// push item at the head
	inline void push(T *item)
	{
		if (head)
		{
			insertBefore(head, item);
		}
		else
			init(item);
	};
	// push item at the tail
	inline void queue(T *item)
	{
		if (head)
		{
			insertBefore(head, item);
		}
		else
			init(item);
	};
	// pop item from the list
	inline T *pop()
	{
		if (!head)
			return (T *)0;
		T *ret = head;
		remove(ret);
		return ret;
	};

	// dequeue an element from the tail
	inline T *dequeue()
	{
		if (!head)
			return (T *)0;
		T *ret = head->prev;
		remove(ret);
		return ret;
	};
};

#endif /*DLLIST_H_*/
