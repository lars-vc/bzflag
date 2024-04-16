//////////////////////////////////////
// PCR, Pointer Chain Randomization //
//////////////////////////////////////
/*
   Copyright (C) 2024 by Lars Van Cauter

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
#pragma once
#define LOGGING
#ifdef LOGGING
#include <cstdio>
#include <typeinfo>
#endif

extern "C" { // should be statically linked
#include <lightning.h>
}

#include <map>      // could be replaced with own map implementation
#include <stdlib.h> // needed for rand() and malloc()

#define TTLU_MAX 240
#define START_LEN 3
#define MAX_LEN 9

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;

typedef ulong (*pulful)(ulong);
typedef short (*psfs)(short);

static jit_state_t *id_transform_jit_state;
static jit_state_t *pick_encode_jit_state;
static jit_state_t *pick_decode_jit_state;

////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////DATASTRUCTURES///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
template <typename T> class ListNode {
  public:
    T val;
    ListNode *next;
    ListNode(T val_) : val(val_), next(nullptr) {}
};
template <typename T> class List {
  public:
    ListNode<T> *head;
    ListNode<T> *tail;
    List() {
        head = nullptr;
        tail = nullptr;
    }
    ~List() {
        ListNode<T> *curr = head;
        while (curr != nullptr) {
            ListNode<T> *tmp = curr;
            curr = curr->next;
            delete tmp;
        }
    }

    T peek_back() { return tail->val; }

    void push_back(List<T> &list) {
        ListNode<T> *curr = list.head;
        while (curr != nullptr) {
            push_back(curr->val);
            curr = curr->next;
        }
    }
    void push_back_no_dup(List<T> &list) {
        ListNode<T> *curr = list.head;
        while (curr != nullptr) {
            if (!contains(curr->val)) {
                push_back(curr->val);
            }
            curr = curr->next;
        }
    }

    T get(int index) {
        if (index == -1) {
            return tail->val;
        }

        ListNode<T> *curr = head;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        }
        return curr->val;
    }

    void push_back(T val) {
        ListNode<T> *node = new ListNode<T>(val);
        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

#ifdef LOGGING
    void print() {
        printf("list: ");
        if (head == nullptr) {
            printf("empty\n");
            return;
        }
        ListNode<ulong> *curr = head;
        while (curr != nullptr) {
            printf("id=%lu ", curr->val);
            curr = curr->next;
        }
        printf("\n");
        printf("head=%lu tail=%lu\n", head->val, tail->val);
    }
#endif

    T pop_back() {
        if (head == nullptr) {
            return 0;
        }
        ListNode<T> *curr = head;
        ListNode<T> *prev = nullptr;
        while (curr->next != nullptr) {
            prev = curr;
            curr = curr->next;
        }
        T val = curr->val;
        if (prev == nullptr) {
            head = nullptr;
            tail = nullptr;
        } else {
            prev->next = nullptr;
            tail = prev;
        }
        delete curr;
        return val;
    }

    bool contains(T val) {
        ListNode<T> *curr = head;
        while (curr != nullptr) {
            if (curr->val == val) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    void erase(T val) {
        ListNode<T> *curr = head;
        ListNode<T> *prev = nullptr;
        while (curr != nullptr) {
            if (curr->val == val) {
                if (prev == nullptr) {
                    head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                if (curr->next == nullptr) {
                    tail = prev;
                }

                ListNode<T> *next = curr->next;
                delete curr;
                curr = next;
                continue;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    void erase1(T val) {
        ListNode<T> *curr = head;
        ListNode<T> *prev = nullptr;
        while (curr != nullptr) {
            if (curr->val == val) {
                if (prev == nullptr) {
                    head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                if (curr->next == nullptr) {
                    tail = prev;
                }
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    bool is_empty() { return head == nullptr; }

    ulong size() {
        ulong count = 0;
        ListNode<T> *curr = head;
        while (curr != nullptr) {
            count++;
            curr = curr->next;
        }
        return count;
    }
};

template <typename T, typename Y> struct Tuple {
    T first;
    Y second;
    Tuple(T first_, Y second_) : first(first_), second(second_) {}
};

inline void _owncpy(void *dest, void *src, size_t n) {
    char *csrc = (char *)src;
    char *cdest = (char *)dest;

    for (uint i = 0; i < n; i++)
        cdest[i] = csrc[i];
}

////////////////////////////////////////////////////////////////////////////////////
// NOTE: Can not accept types larger than 8 bytes
template <typename T> class XOR {
  private:
    unsigned long mask = (ulong)rand() * (ulong)rand();
    unsigned long obfus_val;

    T deobfuscate() const { return static_cast<T>(obfus_val ^ mask); }
    void obfuscate(T newval) {
        obfus_val = mask ^ static_cast<unsigned long>(newval);
    }

  public:
    XOR() { obfuscate(0); }
    XOR(T val) { obfuscate(val); }
    XOR(XOR<T> const &x) { obfuscate(x.val()); }
    ~XOR() {}

    XOR &operator=(T val) {
        obfuscate(val);
        return *this;
    }
    XOR &operator=(XOR &val) { return operator=(val.deobfuscate()); }
    XOR operator+(XOR add) {
        T val = deobfuscate();
        T valadd = add.deobfuscate();
        XOR p(val + valadd);
        return p;
    }
    XOR operator+(T add) {
        T val = deobfuscate();
        XOR p(val + add);
        return p;
    }
    XOR &operator+=(T add) {
        obfuscateAdd(add);
        return *this;
    }
    XOR &operator++(int) { return operator+=(1); }
    XOR operator-(XOR add) { return operator+(-add); }
    XOR operator-(T add) { return operator+(-add); }
    XOR &operator-=(T add) { return operator+=(-add); }
    XOR &operator--(int) { return operator-=(1); }
    template <typename C> bool operator>=(C comp) {
        return (deobfuscate() >= comp);
    }
    template <typename C> bool operator>=(XOR<C> comp) {
        return (deobfuscate() >= comp.val());
    }
    template <typename C> bool operator>(C comp) {
        return (deobfuscate() > comp);
    }
    template <typename C> bool operator>(XOR<C> comp) {
        return (deobfuscate() > comp.val());
    }
    template <typename C> bool operator<=(C comp) {
        return (deobfuscate() <= comp);
    }
    template <typename C> bool operator<=(XOR<C> comp) {
        return (deobfuscate() <= comp.val());
    }
    template <typename C> bool operator<(C comp) {
        return (deobfuscate() < comp);
    }
    template <typename C> bool operator<(XOR<C> comp) {
        return (deobfuscate() < comp.val());
    }
    template <typename C> bool operator==(C comp) {
        return (deobfuscate() == comp);
    }
    template <typename C> bool operator==(XOR<C> comp) {
        return (deobfuscate() == comp.val());
    }
    template <typename C> XOR<T> operator%(C comp) {
        return XOR<T>(deobfuscate() % comp);
    }
    template <typename C> XOR<T> operator%(XOR<C> comp) {
        return XOR<T>(deobfuscate() % comp.val());
    }
    template <typename C> XOR<T> operator/(C comp) {
        return XOR<T>(deobfuscate() / comp);
    }
    template <typename C> XOR<T> operator/(XOR<C> comp) {
        return XOR<T>(deobfuscate() / comp.val());
    }
    template <typename C> XOR<T> operator*(C comp) {
        return XOR<T>(deobfuscate() * comp);
    }
    template <typename C> XOR<T> operator*(XOR<C> comp) {
        return XOR<T>(deobfuscate() * comp.val());
    }

    T val() { return deobfuscate(); }
    const T val() const { return deobfuscate(); }
    void obfuscateAdd(T add) { obfuscate(deobfuscate() + add); }
};
template <typename T> XOR<T> operator-(const T a, const XOR<T> &b) {
    return XOR<T>(a - b.val());
}
template <typename T> XOR<T> operator+(const T a, const XOR<T> &b) {
    return XOR<T>(a + b.val());
}

////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////POINTERCHAINING//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
template <typename T> struct Dummy {
    ulong id;
    Dummy<T> *d1;
    Dummy<T> *d2;
    Dummy<T> *d3;
    T *tail;
};

template <typename T> struct DummyHead {
    Dummy<T> *d;

    XOR<ulong> ttlu;
    XOR<uint> ttlu_max;
    XOR<ulong> times_updated;
    XOR<uint> max_len;

    XOR<ulong> parent_id;
    List<ulong> child_ids;

    bool is_obj;
};

struct ChainOptions {
    ushort max_chain_length = MAX_LEN;
    uint max_time_til_update = TTLU_MAX;
    uint amount_children = 0;

    ChainOptions() {}
    // NOTE: added underscore to not have warning about shadowing
    ChainOptions(ushort max_chain_length_, uint max_time_til_update_ = TTLU_MAX,
                 uint amount_children_ = 0)
        : max_chain_length(max_chain_length_),
          max_time_til_update(max_time_til_update_),
          amount_children(amount_children_) {}
};

class Overseer {
  private:
    template <typename T> Dummy<T> *create_path_rec(int depth, int max_depth) {
        Dummy<T> *d = new Dummy<T>();
        d->id = totalcounter;
        totalcounter++;
        if (depth < max_depth) {
            d->d1 = create_path_rec<T>(depth + 1, max_depth);
            d->d2 = create_path_rec<T>(depth + 1, max_depth);
            d->d3 = create_path_rec<T>(depth + 1, max_depth);
        }
        if (depth == max_depth) {
            d->tail = (T *)malloc(sizeof(T));
        }

        return d;
    }

    template <typename T> void free_path_rec(Dummy<T> *d) {
        if (d->d1 != nullptr) {
            free_path_rec(d->d1);
            free_path_rec(d->d2);
            free_path_rec(d->d3);
        } else {
            free(d->tail);
        }
        erase_el(d->id);
        delete d;
    }

    template <typename T> void check_and_update_head(DummyHead<T> *head) {
        if (due_for_path_update.count(head->d->id)) {
            update_path_dummy_erase(head->d);
            List<ulong> *list = due_for_path_update[head->d->id];
            head->ttlu = time;
            head->times_updated += 1;

            // add children to update list
            ListNode<ulong> *curr = head->child_ids.head;
            while (curr != nullptr) {
                if (!list->contains(curr->val)) {
                    List<ulong> *l = new List<ulong>();
                    l->push_back(head->d->id);
                    if (!due_for_path_update.count(curr->val))
                        due_for_path_update[curr->val] = l;
                    else
                        due_for_path_update[curr->val]->push_back_no_dup(*l);
                }
                curr = curr->next;
            }

            // add parent to update list
            if (head->parent_id.val() != (ulong)-1 &&
                !list->contains(head->parent_id.val())) {
                List<ulong> *l = new List<ulong>();
                l->push_back(head->d->id);
                if (!due_for_path_update.count(head->parent_id.val()))
                    due_for_path_update[head->parent_id.val()] = l;
                else
                    due_for_path_update[head->parent_id.val()]
                        ->push_back_no_dup(*l);
            }

            // delete prev entry
            due_for_path_update.erase(head->d->id);
            delete list;
            // due_for_path_update.push_back_no_dup(head->child_ids);

            // Make chain longer over time
            // every 10th updated increase to a max len of 10
#ifdef LOGGING
            printf("Updated path: id=%lu", head->d->id);
#endif
            if (head->times_updated % 10 == (ulong)0 &&
                head->times_updated + START_LEN * 10 <= head->max_len * 10) {
                // TODO: decreasing lengths?
                change_chain_len(head->d,
                                 (head->times_updated / 10) + START_LEN);
#ifdef LOGGING
                printf(" and increased length to %ld",
                       (head->times_updated.val() / 10) + START_LEN);
#endif
            }
#ifdef LOGGING
            printf("\nlist: ");
            std::map<ulong, List<ulong> *>::iterator it;
            for (it = due_for_path_update.begin();
                 it != due_for_path_update.end(); it++) {
                printf("id=%lu reason{ ", it->first);
                ListNode<ulong> *log = it->second->head;
                while (log != nullptr) {
                    printf("%lu ", log->val);
                    log = log->next;
                }
                printf("}, ");
            }
            printf("\n");
#endif
        }
    }

    template <typename T>
    void change_chain_rec(Dummy<T> *d, int currlen, int len) {
        if (d->d1 == nullptr) {
            if (currlen < len) {
                d->d1 = new Dummy<T>();
                d->d1->id = totalcounter;
                totalcounter++;
                d->d2 = new Dummy<T>();
                d->d2->id = totalcounter;
                totalcounter++;
                d->d3 = new Dummy<T>();
                d->d3->id = totalcounter;
                totalcounter++;

                free(d->tail);

            } else if (currlen == len) {
                d->tail = (T *)malloc(sizeof(T));
                return;
            } else {
#ifdef LOGGING
                printf("ERROR: currlen > len, %d, %d\n", currlen, len);
#endif
                return;
            }
        }
        change_chain_rec(d->d1, currlen + 1, len);
        change_chain_rec(d->d2, currlen + 1, len);
        change_chain_rec(d->d3, currlen + 1, len);
    }

    template <typename T> void change_chain_len(Dummy<T> *d, int len) {
        T *tmp = (T *)malloc(sizeof(T));
        _owncpy(tmp, resolve_dummy(d)->tail, sizeof(T));
        erase_path_dummy(d);
        change_chain_rec(d, 0, len);
        update_path_dummy(d, tmp);
        free(tmp);
    }
    template <typename T, typename C>
    void change_chain_len(Dummy<T> *d, XOR<C> len) {
        change_chain_len(d, len.val());
    }

    template <typename T> Dummy<T> *create_path(int len) {
        Dummy<T> *d = create_path_rec<T>(0, len);

        parent_stack.push_back(d->id);
        void *memory = malloc(sizeof(T));
        T *valptr = new (memory) T();
        parent_stack.pop_back();

        update_path_dummy(d, valptr);
        paths_created++;
#ifdef LOGGING
        printf("Created path: id=%lu name=%s count=%lu\n", d->id,
               typeid(*valptr).name(), paths_created);
#endif
        // we do new and free to avoid calling the destructor of T which would
        // kill the protected<float>s it possesses
        free(memory);
        return d;
    }

    template <typename T> void change_value_dummy(Dummy<T> *d, T val) {
        _owncpy(resolve_dummy(d)->tail->val, &val, sizeof(T));
    }

    template <typename T> void free_path(Dummy<T> *d) {
#ifdef LOGGING
        printf("Freed reg:        id=%lu\n", d->id);
#endif
        if (!due_for_path_update.count(d->id)) {
            delete due_for_path_update[d->id];
            due_for_path_update.erase(d->id);
        }

        free_path_rec(d);
    }

    template <typename T> void free_path_obj(Dummy<T> *d) {
#ifdef LOGGING
        printf("Freed obj:        id=%lu\n", d->id);
#endif
        // call destructor of the obj
        Dummy<T> *special = resolve_dummy(d);
        special->tail->~T();

        if (!due_for_path_update.count(d->id)) {
            delete due_for_path_update[d->id];
            due_for_path_update.erase(d->id);
        }

        free_path_rec(d);
    }

    template <typename T> Dummy<T> *resolve_dummy(Dummy<T> *d) {
        short pick = get_el(d->id);
        Dummy<T> *next = d;
        while (pick) {
            switch (pick) {
            case 1:
                next = next->d1;
                break;
            case 2:
                next = next->d2;
                break;
            case 3:
                next = next->d3;
                break;
            }
            pick = get_el(next->id);
        }
        return next;
    }

    template <typename T> T *resolve_dummy_val(Dummy<T> *d) {
        return resolve_dummy(d)->tail;
    }

    template <typename T> void update_path_dummy(Dummy<T> *d) {
        T *val = resolve_dummy_val(d);
        update_path_dummy(d, val);
    }

    template <typename T> Dummy<T> *erase_path_dummy(Dummy<T> *d) {
        // resolve and erase from catalog
        short pick = get_el(d->id);
        erase_el(d->id);
        Dummy<T> *next = d;
        while (pick) {
            switch (pick) {
            case 1:
                next = next->d1;
                break;
            case 2:
                next = next->d2;
                break;
            case 3:
                next = next->d3;
                break;
            }
            pick = get_el(next->id);
            erase_el(next->id);
        }
        return next;
    }
    template <typename T> void update_path_dummy_erase(Dummy<T> *d) {
        // resolve and erase from catalog
        T *val = erase_path_dummy(d)->tail;
        update_path_dummy(d, val);
    }

    template <typename T> void update_path_dummy(Dummy<T> *d, T *val) {
        Dummy<T> *curr = d;
        while (curr->d1 != nullptr) {
            short pick = (rand() % 3) + 1;
            set_el(curr->id, pick);
            switch (pick) {
            case 1:
                curr = curr->d1;
                break;
            case 2:
                curr = curr->d2;
                break;
            case 3:
                curr = curr->d3;
                break;
            }
        }
        set_el(curr->id, 0);
        _owncpy(curr->tail, val, sizeof(T));
    }

    short get_el(ulong id) {
        short pick = catalog[transform_id(id)];
        return decode_pick(pick);
    }

    void set_el(ulong id, short pick) {
        short r = rand() << 7;
        catalog[transform_id(id)] = r | encode_pick(pick);
    }

    void erase_el(ulong id) { catalog.erase(transform_id(id)); }

    // id to correct path
    std::map<ulong, short> catalog;
    // update path on next resolve
    std::map<ulong, List<ulong> *> due_for_path_update;
    ulong paths_created = 0;
    ulong totalcounter = 0;
    ulong time = 0;
    pulful transform_id;
    psfs encode_pick;
    psfs decode_pick;
    List<ulong> child_stack;
    List<ulong> parent_stack;

    Overseer() {
        // Create a jit function that will transform the id. This is scheme that
        // I devised to see it in C code see norg files
        init_jit(NULL);
        id_transform_jit_state = jit_new_state();
        pick_encode_jit_state = jit_new_state();
        pick_decode_jit_state = jit_new_state();

        _jit_prolog(id_transform_jit_state);
        _jit_getarg_l(id_transform_jit_state, _RAX,
                      _jit_arg(id_transform_jit_state, jit_code_arg_l));
        // res = 0
        _jit_new_node_www(id_transform_jit_state, jit_code_xorr, _R11, _R11,
                          _R11);

        // res |= id << 60;
        _jit_new_node_www(id_transform_jit_state, jit_code_lshi, _R10, _RAX,
                          60);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R11, _R11,
                          _R10);

        // res |= id >> 60;
        _jit_new_node_www(id_transform_jit_state, jit_code_lroti, _R10, _RAX,
                          4);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R10, _R10,
                          0xF);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R11, _R11,
                          _R10);

        // res |= ((id >> 4 & 0xFF) ^ (((id & 0xF) << 4) | (id >> 60 & 0xF))) <<
        // 4;
        _jit_new_node_www(id_transform_jit_state, jit_code_rshi, _R10, _RAX, 4);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R13, _R10,
                          0xFF);
        //
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R14, _RAX,
                          0xF);
        _jit_new_node_www(id_transform_jit_state, jit_code_lshi, _R14, _R14, 4);
        //
        _jit_new_node_www(id_transform_jit_state, jit_code_lroti, _R10, _RAX,
                          4);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R10, _R10,
                          0xF);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R10, _R14,
                          _R10);
        //
        _jit_new_node_www(id_transform_jit_state, jit_code_xorr, _R10, _R13,
                          _R10);
        _jit_new_node_www(id_transform_jit_state, jit_code_lshi, _R10, _R10, 4);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R11, _R11,
                          _R10);

        // res |= (id << 44 & 0x0F00000000000000);
        _jit_new_node_www(id_transform_jit_state, jit_code_lshi, _R10, _RAX,
                          44);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R10, _R10,
                          0x0F00000000000000);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R11, _R11,
                          _R10);

        // res |= (id >> 44 & 0x000000000000F000);
        _jit_new_node_www(id_transform_jit_state, jit_code_lroti, _R10, _RAX,
                          20);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R10, _R10,
                          0x000000000000F000);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R11, _R11,
                          _R10);

        // ulong f = (id >> 16 ^ id >> 24) & 0xFF;
        // res |= ((f | id >> 24) & 0xFF) << 24 | f << 16;
        _jit_new_node_www(id_transform_jit_state, jit_code_rshi, _R13, _RAX,
                          16);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R13, _R13,
                          0xFF);
        _jit_new_node_www(id_transform_jit_state, jit_code_rshi, _R14, _RAX,
                          24);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R14, _R14,
                          0xFF);
        _jit_new_node_www(id_transform_jit_state, jit_code_xorr, _R10, _R13,
                          _R14);
        //
        _jit_new_node_www(id_transform_jit_state, jit_code_rshi, _R13, _RAX,
                          24);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R13, _R13,
                          0xFF);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R13, _R10,
                          _R13); // LARS
        _jit_new_node_www(id_transform_jit_state, jit_code_lshi, _R13, _R13,
                          24);
        _jit_new_node_www(id_transform_jit_state, jit_code_lshi, _R10, _R10,
                          16);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R10, _R13,
                          _R10);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R11, _R11,
                          _R10);

        // res |= (id ^ 0x00FF00FF00000000) & 0x00FF00FF00000000;
        _jit_new_node_www(id_transform_jit_state, jit_code_xori, _R10, _RAX,
                          0x00FF00FF00000000);
        _jit_new_node_www(id_transform_jit_state, jit_code_andi, _R10, _R10,
                          0x00FF00FF00000000);
        _jit_new_node_www(id_transform_jit_state, jit_code_orr, _R11, _R11,
                          _R10);

        _jit_retr(id_transform_jit_state, _R11,
                  jit_code_retr_l); /*      retr   R2           */

        transform_id = (pulful)_jit_emit(id_transform_jit_state);
        _jit_clear_state(id_transform_jit_state);

        // pick encode
        _jit_prolog(pick_encode_jit_state);
        _jit_getarg_l(pick_encode_jit_state, _RAX,
                      _jit_arg(pick_encode_jit_state, jit_code_arg_l));
        // 1) <<1
        _jit_new_node_www(pick_encode_jit_state, jit_code_lshi, _RAX, _RAX, 1);
        // 2) copy bit 1 and 2 to 2 and 3
        _jit_new_node_www(pick_encode_jit_state, jit_code_andi, _R10, _RAX, 6);
        _jit_new_node_www(pick_encode_jit_state, jit_code_ori, _RAX, _RAX, 12);
        _jit_new_node_www(pick_encode_jit_state, jit_code_lshi, _R10, _R10, 1);
        _jit_new_node_www(pick_encode_jit_state, jit_code_ori, _R10, _R10, 3);
        _jit_new_node_www(pick_encode_jit_state, jit_code_andr, _RAX, _RAX,
                          _R10);
        // 3) nand 2 and 3 and and with rest
        _jit_new_node_www(pick_encode_jit_state, jit_code_andi, _R10, _RAX, 8);
        _jit_new_node_www(pick_encode_jit_state, jit_code_andi, _R11, _RAX, 4);
        _jit_new_node_www(pick_encode_jit_state, jit_code_lshi, _R11, _R11, 1);
        // for 0, nor
        _jit_new_node_www(pick_encode_jit_state, jit_code_orr, _RBX, _R11,
                          _R10);
        _jit_new_node_www(pick_encode_jit_state, jit_code_xori, _RBX, _RBX, 8);
        _jit_new_node_www(pick_encode_jit_state, jit_code_rroti, _RBX, _RBX, 3);
        // nand = and+xor
        _jit_new_node_www(pick_encode_jit_state, jit_code_andr, _R11, _R10,
                          _R11);
        _jit_new_node_www(pick_encode_jit_state, jit_code_xori, _R11, _R11, 8);
        //
        _jit_new_node_www(pick_encode_jit_state, jit_code_ori, _R11, _R11,
                          65527);
        _jit_new_node_www(pick_encode_jit_state, jit_code_andr, _RAX, _RAX,
                          _R11);
        _jit_new_node_www(pick_encode_jit_state, jit_code_rshi, _R11, _R11, 1);
        _jit_new_node_www(pick_encode_jit_state, jit_code_andr, _RAX, _RAX,
                          _R11);
        _jit_new_node_www(pick_encode_jit_state, jit_code_rshi, _R11, _R11, 1);
        _jit_new_node_www(pick_encode_jit_state, jit_code_andr, _RAX, _RAX,
                          _R11);
        _jit_new_node_www(pick_encode_jit_state, jit_code_orr, _RAX, _RAX,
                          _RBX);
        // set 7thbit
        _jit_new_node_www(pick_encode_jit_state, jit_code_ori, _RAX, _RAX, 128);
        _jit_retr(pick_encode_jit_state, _RAX, jit_code_retr_l);

        encode_pick = (psfs)_jit_emit(pick_encode_jit_state);
        _jit_clear_state(pick_encode_jit_state);

        // pick decode
        _jit_prolog(pick_decode_jit_state);
        _jit_getarg_l(pick_decode_jit_state, _RAX,
                      _jit_arg(pick_decode_jit_state, jit_code_arg_l));

        _jit_new_node_ww(pick_decode_jit_state, jit_code_ctzr, _RAX, _RAX);
        _jit_new_node_ww(pick_decode_jit_state, jit_code_popcntr, _RAX, _RAX);

        _jit_retr(pick_decode_jit_state, _RAX, jit_code_retr_l);

        decode_pick = (psfs)_jit_emit(pick_decode_jit_state);
        _jit_clear_state(pick_decode_jit_state);
    }

    ~Overseer() {
        _jit_destroy_state(id_transform_jit_state);
        _jit_destroy_state(pick_encode_jit_state);
        _jit_destroy_state(pick_decode_jit_state);
        finish_jit();
    }

  public:
    template <typename T>
    DummyHead<T> *create_head(ChainOptions options, bool is_obj) {
        DummyHead<T> *head = new DummyHead<T>();
        head->ttlu_max = options.max_time_til_update;
        head->times_updated = 0;
        head->is_obj = is_obj;
        head->max_len = options.max_chain_length > START_LEN
                            ? options.max_chain_length
                            : START_LEN;
        if (parent_stack.is_empty())
            head->parent_id = -1;
        else
            head->parent_id = parent_stack.peek_back();
        head->d = create_path<T>(START_LEN); // this will create children
        head->child_ids = List<ulong>();
        if (!child_stack.is_empty()) {
            for (uint i = 0; i < options.amount_children; i++) {
                head->child_ids.push_back(child_stack.pop_back());
            }
        }
        child_stack.push_back(head->d->id);
        return head;
    }

    template <typename T> void free_head(DummyHead<T> *head) {
        if (head->is_obj)
            free_path_obj(head->d);
        else
            free_path(head->d);
        delete head;
    }

    template <typename T> Dummy<T> *resolve_head(DummyHead<T> *head) {
        // check if dummy is due for chain update
        if (time - head->ttlu >= head->ttlu_max) {
            // don't add if already in list
            if (!due_for_path_update.count(head->d->id)) {
                List<ulong> *l = new List<ulong>();
                due_for_path_update[head->d->id] = l;
            }
        }
        check_and_update_head(head);
        return resolve_dummy(head->d);
    }

    template <typename T> T *resolve_head_val(DummyHead<T> *head) {
        return resolve_head(head)->tail;
    }

    // Could decide here whether to update chain or not based on knowledge of
    // change of value
    template <typename T> void change_value(DummyHead<T> *head, T val) {
        Dummy<T> *d = resolve_head(head);
        _owncpy(d->tail, &val, sizeof(T));
    }

    void inc_time(long inc = 1) { time += inc; }

    static Overseer &get() {
        static Overseer overseer;
        return overseer;
    }
};

inline void increment_time(long dt = 1) { Overseer::get().inc_time(dt); }
// inline float xorUnsignedLongAndFloat(unsigned long ulongValue,
//                                      float floatValue) {
//     // Reinterpret the float as an unsigned long
//     unsigned long *floatAsULong = (unsigned long *)&floatValue;
//
//     // XOR the two unsigned long integers
//     unsigned long resultULong = ulongValue ^ *floatAsULong;
//
//     // Reinterpret the result as a float
//     float resultFloat = *(float *)&resultULong;
//
//     return resultFloat;
// }
// inline float xorUnsignedLongAndFloat(unsigned long ulongValue,
//                                      float floatValue) {
//     // Reinterpret the float as an unsigned long
//     int *floatAsULong = (int *)&floatValue;
//
//     // XOR the two unsigned long integers
//     int resultULong = ulongValue ^ *floatAsULong;
//
//     // Reinterpret the result as a float
//     float resultFloat = *(float *)&resultULong;
//
//     return resultFloat;
// }
template <typename T> inline T _xormask(unsigned long mask, T val) {
    switch (sizeof(T)) {
    case sizeof(char): {
        char store;
        _owncpy(&store, &val, sizeof(char));
        store ^= mask;
        T res;
        _owncpy(&res, &store, sizeof(char));
        return res;
    }
    case sizeof(short): {
        short store;
        _owncpy(&store, &val, sizeof(short));
        store ^= mask;
        T res;
        _owncpy(&res, &store, sizeof(short));
        return res;
    }
    case sizeof(int): {
        int store;
        _owncpy(&store, &val, sizeof(int));
        store ^= mask;
        T res;
        _owncpy(&res, &store, sizeof(int));
        return res;
    }
    case sizeof(long): {
        long store;
        _owncpy(&store, &val, sizeof(long));
        store ^= mask;
        T res;
        _owncpy(&res, &store, sizeof(long));
        return res;
    }
    default:
        return val;
    };
}

// NOTE: Type T should be a primitive
template <typename T> class Protected {
  private:
    DummyHead<T> *head;
    ulong mask = (ulong)rand() * (ulong)rand();

    T deobfuscate() const {
        return _xormask(mask, *Overseer::get().resolve_head_val(head));
    }
    void obfuscate(T val) {
        Overseer::get().change_value(head, _xormask(mask, val));
    }

  public:
    Protected(ChainOptions options = ChainOptions()) {
        head = Overseer::get().create_head<T>(options, false);
    }
    Protected(Protected<T> const &p) {
        head = Overseer::get().create_head<T>(ChainOptions(), false);
        obfuscate(p.val());
    }

    ~Protected() { Overseer::get().free_head(head); }

    Protected &operator=(T val) {
        obfuscate(val);
        return *this;
    }
    Protected &operator=(Protected &val) {
        return operator=(val.deobfuscate());
    }

    Protected &operator+=(T add) {
        obfuscate(deobfuscate() + add);
        return *this;
    }
    Protected &operator++(int) { return this->operator+=(1); }
    Protected &operator-=(T add) { return this->operator+=(-add); }
    Protected &operator--(int) { return this->operator-=(1); }
    Protected &operator*=(T mul) {
        obfuscate(deobfuscate() * mul);
        return *this;
    }
    Protected &operator/=(T div) {
        obfuscate(deobfuscate() / div);
        return *this;
    }
    Protected &operator%=(T mod) {
        obfuscate(deobfuscate() % mod);
        return *this;
    }
    Protected &operator&=(T andd) {
        obfuscate(deobfuscate() & andd);
        return *this;
    }
    Protected &operator|=(T orr) {
        obfuscate(deobfuscate() & orr);
        return *this;
    }
    Protected &operator<<=(T shift) {
        obfuscate(deobfuscate() << shift);
        return *this;
    }
    Protected &operator>>=(T shift) {
        obfuscate(deobfuscate() >> shift);
        return *this;
    }
    Protected &operator^=(T xorr) {
        obfuscate(deobfuscate() ^ xorr);
        return *this;
    }

    T val() { return deobfuscate(); }
    const T val() const { return deobfuscate(); }

    // NOTE: Only use this when you know what you are doing, as this could
    // induce semi undefined behavior
    // also these conflicht with applying XOR protection to this
    // T *ref() { return deobfuscate_ptr(); }
    // T *operator&() { return ref(); }
    // T *ref() const { return deobfuscate_ptr(); }
    // T *operator&() const { return ref(); }

    operator T() { return deobfuscate(); }
    operator const T() const { return deobfuscate(); }
};

// NOTE: Type T should be a class or struct
template <typename T> class ProtectedPtr {
  private:
    DummyHead<T> *head;
    T &deobfuscate() const { return *Overseer::get().resolve_head_val(head); }
    T *deobfuscate_ptr() const {
        return Overseer::get().resolve_head_val(head);
    }
    void obfuscate(T val) { Overseer::get().change_value(head, val); }
    void obfuscate(T *ptr) { Overseer::get().change_value(head, *ptr); }

  public:
    ProtectedPtr(ChainOptions options = ChainOptions()) {
        head = Overseer::get().create_head<T>(options, true);
    }
    ProtectedPtr(ProtectedPtr const &p) {
        head = Overseer::get().create_head<T>(MAX_LEN, true);
        obfuscate(p.val());
    }

    ~ProtectedPtr() { Overseer::get().free_head(head); }

    ProtectedPtr &operator=(T val) {
        obfuscate(val);
        return *this;
    }
    ProtectedPtr &operator=(T *val) {
        obfuscate(val);
        return *this;
    }
    ProtectedPtr &operator=(ProtectedPtr &val) {
        return operator=(val.deobfuscate());
    }

    T *operator->() { return deobfuscate_ptr(); }
    const T *operator->() const { return deobfuscate_ptr(); }

    T &operator*() { return deobfuscate(); }
    const T &operator*() const { return deobfuscate(); }

    operator T *() { return deobfuscate_ptr(); }
    operator const T *() const { return deobfuscate_ptr(); }
};
////////////////////////////////////////////////////////////////////////////////////
