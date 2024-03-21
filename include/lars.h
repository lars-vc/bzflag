#pragma once
// #include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
extern "C" {
#include <lightning.h>
}
#include <map>
#include <stdlib.h>
#include <unistd.h>

#define protectfunc /*PROTECTOR_FUNCTION_PRIO=5*/
#define PERF_MODE 0
#define TTLU_MAX 240
#define START_LEN 3
#define MAX_LEN 9
#define LOGGING true

typedef ulong (*pulful)(ulong);
typedef short (*psfs)(short);
static jit_state_t *jit1;
static jit_state_t *jit2;
static jit_state_t *jit3;
///////////////////////////////////////////
// protector
///////////////////////////////////////////
// class ProtectedFloat {
//   protected:
//     float unprotected_val = 0.0f;
//     virtual float deobfuscate() const { return unprotected_val; };
//     virtual void obfuscate(float val) { unprotected_val = val; }
//
//   public:
//     ProtectedFloat() { obfuscate(0); }
//     ProtectedFloat(float val) { obfuscate(val); }
//     virtual ~ProtectedFloat() = default;
//
//     // OPERATORS
//     ProtectedFloat &operator=(float val) {
//         obfuscate(val);
//         return *this;
//     }
//     ProtectedFloat &operator=(ProtectedFloat val) {
//         return operator=(val.deobfuscate());
//     }
//     ProtectedFloat operator+(ProtectedFloat add) {
//         float val = deobfuscate();
//         float valadd = add.deobfuscate();
//         ProtectedFloat p(val + valadd);
//         return p;
//     }
//     ProtectedFloat operator+(float add) {
//         float val = deobfuscate();
//         ProtectedFloat p(val + add);
//         return p;
//     }
//     ProtectedFloat operator-(ProtectedFloat add) {
//         float val = deobfuscate();
//         float valadd = add.deobfuscate();
//         ProtectedFloat p(val - valadd);
//         return p;
//     }
//     ProtectedFloat operator-(float add) {
//         float val = deobfuscate();
//         ProtectedFloat p(val - add);
//         return p;
//     }
//     ProtectedFloat &operator+=(float add) {
//         obfuscateAdd(add);
//         return *this;
//     }
//     ProtectedFloat &operator++(int) { return this->operator+=(1); }
//     ProtectedFloat &operator-=(float add) { return this->operator+=(-add); }
//     ProtectedFloat &operator--(int) { return this->operator-=(1); }
//     bool operator>=(float comp) { return (deobfuscate() >= comp); }
//     bool operator<=(float comp) { return (deobfuscate() <= comp); }
//     bool operator>(float comp) { return (deobfuscate() > comp); }
//     bool operator<(float comp) { return (deobfuscate() < comp); }
//     operator bool() const { return deobfuscate() != 0; }
//     // Maybe this can work?
//     // operator float() const { return deobfuscate(); }
//
//     float val() { return deobfuscate(); }
//     const float val() const { return deobfuscate(); }
//     void obfuscateAdd(float add) { obfuscate(deobfuscate() + add); }
//     void obfuscateAdd(float add, float linenr) {
//         obfuscate(deobfuscate() + add);
//     }
// };
// class SplitFloat /*: public ProtectedFloat */ {
//   private:
//     float val0 = 0.0f;
//     float val1 = 0.0f;
//     float val2 = 0.0f;
//     float val3 = 0.0f;
//     float deobfuscate() const { return val0 + val1 + val2 + val3; }
//     void obfuscate(float val) {
//         // printf("val: %f\n", val);
//         // int r = rand();
//         // printf("%d\n", r);
//         // printf("%d\n", r%static_cast<int>(val));
//         // printf("%f\n", (float)(r%(int)val));
//         val0 = val - (float)(rand() % 1000);
//         // val1 = val / 2;
//         // val2 = val / 3;
//         val1 = val - (float)(rand() % 1000);
//         val2 = val - (float)(rand() % 1000);
//         val3 = val - val0 - val1 - val2;
//         // printf("val0: %f, val1: %f, val2: %f, val3: %f\n", val0, val1,
//         val2,
//         //        val3);
//     }
//
//   public:
//     SplitFloat() {}          //: ProtectedFloat() {}
//     SplitFloat(float val) {} // : ProtectedFloat(val) {}
//     ~SplitFloat() = default;
//
//     SplitFloat &operator=(float val) {
//         obfuscate(val);
//         return *this;
//     }
//     SplitFloat &operator=(SplitFloat val) {
//         return operator=(val.deobfuscate());
//     }
//     void obfuscateAdd(float add) {
//         printf("add %f", add);
//         val3 += add;
//     }
//     void obfuscateAdd(float add, int linenr) {
//         if (linenr % 4 == 0) {
//             val0 += add;
//         } else if (linenr % 4 == 1) {
//             val1 += add;
//         } else if (linenr % 4 == 2) {
//             val2 += add;
//         } else if (linenr % 4 == 3) {
//             val3 += add;
//         }
//     }
//     float val() { return deobfuscate(); }
//     const float val() const { return deobfuscate(); }
// };
//
// class ProtectedInt {
//   protected:
//     int unprotected_val;
//     virtual int deobfuscate() const { return unprotected_val; };
//     virtual void obfuscate(int val) { unprotected_val = val; }
//
//   public:
//     ProtectedInt() { obfuscate(0); }
//     ProtectedInt(int val) { obfuscate(val); }
//     virtual ~ProtectedInt() = default;
//
//     // OPERATORS
//     ProtectedInt &operator=(int val) {
//         obfuscate(val);
//         return *this;
//     }
//     ProtectedInt &operator=(ProtectedInt val) {
//         return operator=(val.deobfuscate());
//     }
//     ProtectedInt operator+(ProtectedInt add) {
//         int val = deobfuscate();
//         int valadd = add.deobfuscate();
//         ProtectedInt p(val + valadd);
//         return p;
//     }
//     ProtectedInt operator+(int add) {
//         int val = deobfuscate();
//         ProtectedInt p(val + add);
//         return p;
//     }
//     ProtectedInt &operator+=(int add) {
//         obfuscateAdd(add);
//         return *this;
//     }
//     ProtectedInt &operator++(int) { return this->operator+=(1); }
//     ProtectedInt &operator-=(int add) { return this->operator+=(-add); }
//     ProtectedInt &operator--(int) { return this->operator-=(1); }
//     operator bool() const { return deobfuscate() != 0; }
//     // Maybe this can work?
//     // operator int() const { return deobfuscate(); }
//
//     int val() { return deobfuscate(); }
//     void obfuscateAdd(int add) { obfuscate(deobfuscate() + add); }
//     void obfuscateAdd(int add, int linenr) { obfuscate(deobfuscate() + add);
//     }
// };
//
// class XORInt : public ProtectedInt {
//   private:
//     int mask = rand();
//     int obfuscated_val;
//     int deobfuscate() const override { return mask ^ obfuscated_val; }
//     void obfuscate(int val) override { obfuscated_val = mask ^ val; }
//     void obfuscateAdd(int add) { obfuscated_val += add; }
//
//   public:
//     XORInt() : ProtectedInt() {}
//     XORInt(int val) : ProtectedInt(val) {}
//     ~XORInt() = default;
// };
//
// class SplitInt : public ProtectedInt {
//   private:
//     int val0;
//     int val1;
//     int val2;
//     int val3;
//     int deobfuscate() const override { return val0 + val1 + val2 + val3; }
//     void obfuscate(int val) override {
//         val0 = val - rand();
//         val1 = val / 2;
//         val2 = val / 3;
//         val3 = val - val0 - val1 - val2;
//     }
//
//   public:
//     SplitInt() : ProtectedInt() {}
//     SplitInt(int val) : ProtectedInt(val) {}
//     ~SplitInt() = default;
//
//     void obfuscateAdd(int add) { val3 += add; }
//     void obfuscateAdd(int add, int linenr) {
//         if (linenr % 4 == 0) {
//             val0 += add;
//         } else if (linenr % 4 == 1) {
//             val1 += add;
//         } else if (linenr % 4 == 2) {
//             val2 += add;
//         } else if (linenr % 4 == 3) {
//             val3 += add;
//         }
//     }
// };
//
// class ChainInt : public ProtectedInt {
//   private:
//     int size;
//     int **p;
//     int **start;
//     int deobfuscate() const override { return **p; }
//     void obfuscate(int val) override { **p = val; }
//     void create_new_path(int val) {
//         start = (int **)malloc(sizeof(int *));
//         p = start;
//         size = rand() % 10 + 1;
//         for (int i = 0; i < size; i++) {
//             int *next = (int *)malloc(sizeof(int *));
//             *p = next;
//             p = (int **)next;
//         }
//         int *next = (int *)malloc(sizeof(int *));
//         *p = next;
//         **p = val;
//     }
//     void free_path() {
//         // clear value out of memory, might not work thanks to compiler
//         obfuscate(0xdeadbeef);
//         p = start;
//         for (int i = 0; i < size + 1; i++) {
//             int *next = *p;
//             free(p);
//             p = (int **)next;
//         }
//         free(p);
//     }
//
//   public:
//     ChainInt() { create_new_path(0); }
//     ChainInt(int val) { create_new_path(val); }
//     ~ChainInt() { free_path(); }
//
//     void newpath() {
//         int val = deobfuscate();
//         free_path();
//         create_new_path(val);
//     }
// };
//
// template <typename T> class vec {
//   private:
//     T **arr;
//     unsigned long curr_len;
//     unsigned long len;
//
//   public:
//     vec() {
//         curr_len = 0;
//         len = 10;
//         arr = new T *[len];
//     }
//     vec(int startlen) : len(startlen) {
//         curr_len = 0;
//         arr = new T[len];
//     }
//     ~vec() {
//         for (unsigned long i = 0; i < curr_len; i++) {
//             delete arr[i];
//         }
//         delete[] arr;
//     }
//     T &operator[](int index) { return *arr[index]; }
//     const T &operator[](int index) const { return *arr[index]; }
//     void push_back(T val) {
//         if (curr_len == len) {
//             len *= 2;
//             T **tmp = new T *[len];
//             for (unsigned long i = 0; i < curr_len; i++) {
//                 tmp[i] = arr[i];
//             }
//             delete[] arr;
//             arr = tmp;
//         }
//         arr[curr_len] = new T(val);
//         curr_len++;
//     }
//     unsigned long size() { return curr_len; }
//     unsigned long get_index(T val) {
//         for (unsigned long i = 0; i < curr_len; i++) {
//             if (*arr[i] == val) {
//                 return i;
//             }
//         }
//         return -1;
//     }
//     bool contains(T val) {
//         for (unsigned long i = 0; i < curr_len; i++) {
//             if (*arr[i] == val) {
//                 return true;
//             }
//         }
//         return false;
//     }
//     void erase(T val) {
//         // TODO: FIX THIS, DOES NOT WORK
//         for (unsigned long i = 0; i < curr_len - 1; i++) {
//             if (*arr[i] == val) {
//                 arr[i] = arr[i + 1];
//                 i--;
//             }
//         }
//         curr_len--;
//     }
// };
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////DATASTRUCTURES///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
template <typename T> class ListNode {
  public:
    T val;
    ListNode *next;
    ListNode(T val) : val(val), next(nullptr) {}
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
                prev = curr;
                curr = curr->next;
                delete curr;
                continue;
            }
            prev = curr;
            curr = curr->next;
        }
    }
};

////////////////////////////////////////////////////////////////////////////////////
// NOTE: Can not accept types larger than 8 bytes
template <typename T> class XOR {
  private:
    // TODO: rand is til intmax not longmax
    unsigned long mask = rand();
    unsigned long obfus_val;
    T deobfuscate() const { return static_cast<T>(obfus_val ^ mask); }
    void obfuscate(T newval) {
        obfus_val = mask ^ static_cast<unsigned long>(newval);
    }

  public:
    XOR() { obfuscate(0); }
    XOR(T val) { obfuscate(val); }
    XOR(XOR<T> const &x) { obfuscate(x.val()); }
    // TODO: LARS
    // XOR(XOR &&p) { printf("TODO MOVE CONSTRUCTOR\n"); }
    ~XOR() {}

    XOR &operator=(T val) {
        obfuscate(val);
        return *this;
    }
    XOR &operator=(XOR &val) { return operator=(val.deobfuscate()); }
    // TODO:
    // XOR &operator=(XOR &&val) {
    //     printf("TODO MOVE assignment\n");
    //     return nullptr;
    // }
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
    // operator bool() const { return deobfuscate() != 0; }
    // Maybe this can work?
    // operator int() const { return deobfuscate(); }

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

//////// POINTER CHAIN STUFF ////////
// TODO: do we use this?
template <typename T> struct DummyTail {
    T *val;
};

template <typename T> struct Dummy {
    unsigned long id;
    Dummy<T> *d1;
    Dummy<T> *d2;
    Dummy<T> *d3;
    DummyTail<T> *tail;
};

template <typename T> struct DummyHead {
    Dummy<T> *d;
    // XOR these
    XOR<ulong> ttlu;
    XOR<uint> ttlu_max;
    XOR<ulong> times_updated;
    XOR<uint> max_len;

    bool is_obj;
};

template <typename T> struct ChainOptions {
    ushort max_chain_length = MAX_LEN;
    uint max_time_til_update = TTLU_MAX;
};

class Overseer {
  private:
    template <typename T> Dummy<T> *create_path_rec(int depth, int max_depth) {
        Dummy<T> *d = new Dummy<T>();
        d->id = totalcounter;
        totalcounter++;
        alloc_sum += sizeof(Dummy<T>);
        if (depth < max_depth) {
            d->d1 = create_path_rec<T>(depth + 1, max_depth);
            d->d2 = create_path_rec<T>(depth + 1, max_depth);
            d->d3 = create_path_rec<T>(depth + 1, max_depth);
        }
        // d->tail = new DummyTail<T>();
        // TODO: do we only initialize obj at maxdepth?
        if (depth == max_depth) {
            d->tail = (DummyTail<T> *)malloc(sizeof(DummyTail<T>));
            alloc_sum += sizeof(DummyTail<T>);
            if (PERF_MODE)
                d->tail->val = nullptr;
            else {
                d->tail->val = (T *)malloc(sizeof(T));
                // d->tail->val = new T();
                alloc_sum += sizeof(T);
            }
        }
        // d->tail->val = nullptr;
        // TODO: LARS
        // d->id = catalog.size();
        // d->id = rand();
        // d->tail->val = rand();
        return d;
    }

    template <typename T> void free_path_rec(Dummy<T> *d, unsigned long id) {
        if (d->d1 != nullptr) {
            free_path_rec(d->d1, id);
            free_path_rec(d->d2, id);
            free_path_rec(d->d3, id);
        } else {
            if (!PERF_MODE) {
                // if (obj) {
                //     delete d->tail->val;
                // } else {
                // free(d->tail->val);
                // }
                if (d->id != id) {
                    free(d->tail->val);
                }
            }
            free(d->tail);
        }
        erase_el(d->id);
        // TODO: fix warning
        delete d;
    }

    template <typename T> void check_and_update_head(DummyHead<T> *head) {
        if (due_for_path_update.contains(head->d->id)) {
            update_path_dummy_erase(
                head->d); //, resolve_dummy(head->d)->tail->val);
            due_for_path_update.erase(head->d->id);
            head->ttlu = time;
            head->times_updated += 1;
            // Make chain longer over time
            // every 10th updated increase to a max len of 10
            if (LOGGING)
                printf("Updated path: id=%lu", head->d->id);
            if (head->times_updated % 10 == 0 &&
                head->times_updated + START_LEN * 10 <= head->max_len * 10) {
                // TODO: decreasing lengths
                change_chain_len(head->d,
                                 (head->times_updated / 10) + START_LEN);
                if (LOGGING)
                    printf(" and increased length to %ld",
                           (head->times_updated.val() / 10) + START_LEN);
            }
            if (LOGGING) {
                printf("\nlist: ");
                ListNode<unsigned long> *curr = due_for_path_update.head;
                while (curr != nullptr) {
                    printf("id=%lu ", curr->val);
                    curr = curr->next;
                }
                printf("\n");
            }
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

                free(d->tail->val);
                free(d->tail);

            } else if (currlen == len) {
                d->tail = (DummyTail<T> *)malloc(sizeof(DummyTail<T>));
                d->tail->val = (T *)malloc(sizeof(T));
                return;
            } else {
                if (LOGGING)
                    printf("ERROR: currlen > len, %d, %d\n", currlen, len);
                return;
            }
        }
        change_chain_rec(d->d1, currlen + 1, len);
        change_chain_rec(d->d2, currlen + 1, len);
        change_chain_rec(d->d3, currlen + 1, len);
    }

    // TODO: decreasing lengths
    template <typename T> void change_chain_len(Dummy<T> *d, int len) {
        T *tmp = (T *)malloc(sizeof(T));
        memcpy(tmp, resolve_dummy(d)->tail->val, sizeof(T));
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
        T *valptr = new T();
        update_path_dummy(d, valptr);
        paths_created++;
        if (LOGGING)
            printf("Created path: id=%lu name=%s count=%lu\n", d->id,
                   typeid(*valptr).name(), paths_created);
        // we do new and free to avoid calling the destructor of T which would
        // kill the protected<float>s it possesses
        // free(valptr);
        // try not to mix new and free
        operator delete(valptr);
        return d;
    }

    template <typename T> void change_value_dummy(Dummy<T> *d, T val) {
        memcpy(resolve_dummy(d)->tail->val, &val, sizeof(T));
    }

    template <typename T> void free_path(Dummy<T> *d) {
        if (LOGGING)
            printf("Freed reg:        id=%lu\n", d->id);
        free_path_rec(d, -1);
    }

    template <typename T> void free_path_obj(Dummy<T> *d) {
        if (LOGGING)
            printf("Freed obj:        id=%lu\n", d->id);
        // call destructor of the obj
        Dummy<T> *special = resolve_dummy(d);
        int id = special->id;
        delete special->tail->val;
        free_path_rec(d, id);
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
        return resolve_dummy(d)->tail->val;
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
        T *val = erase_path_dummy(d)->tail->val;
        // TODO: isn't this the same as update_path_dummy?
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
        if (PERF_MODE)
            curr->tail->val = (T *)malloc(sizeof(T));
        memcpy(curr->tail->val, val, sizeof(T));
        if (PERF_MODE)
            free(val);
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
        if (PERF_MODE)
            curr->tail->val = (T *)malloc(sizeof(T));
        memcpy(curr->tail->val, val, sizeof(T));
        if (PERF_MODE)
            free(val);
    }

    // TODO: ask to inline this?
    short get_el(unsigned long id) {
        short pick = catalog[transform_id(id)];
        return decode_pick(pick);
    }

    void set_el(unsigned long id, short pick) {
        catalog[transform_id(id)] = encode_pick(pick);
    }

    void erase_el(unsigned long id) { catalog.erase(transform_id(id)); }

    // id to correct path
    // std::map<unsigned long, short> catalog;
    std::map<unsigned long, short> catalog;
    // update path on next resolve
    List<unsigned long> due_for_path_update;
    unsigned long paths_created = 0;
    unsigned long totalcounter = 0;
    unsigned long alloc_sum = 0;
    unsigned long time = 0;
    pulful transform_id;
    psfs encode_pick;
    psfs decode_pick;

  public:
    Overseer() {
        // Create a jit function that will transform the id. This is scheme that
        // I devised to see it in C code see norg files
        init_jit(NULL);
        jit1 = jit_new_state();
        jit2 = jit_new_state();
        jit3 = jit_new_state();

        _jit_prolog(jit1);
        _jit_getarg_l(jit1, _RAX, _jit_arg(jit1, jit_code_arg_l));
        // res = 0
        _jit_new_node_www(jit1, jit_code_xorr, _R11, _R11, _R11);

        // res |= id << 60;
        _jit_new_node_www(jit1, jit_code_lshi, _R10, _RAX, 60);
        _jit_new_node_www(jit1, jit_code_orr, _R11, _R11, _R10);

        // res |= id >> 60;
        _jit_new_node_www(jit1, jit_code_lroti, _R10, _RAX, 4);
        _jit_new_node_www(jit1, jit_code_andi, _R10, _R10, 0xF);
        _jit_new_node_www(jit1, jit_code_orr, _R11, _R11, _R10);

        // res |= ((id >> 4 & 0xFF) ^ (((id & 0xF) << 4) | (id >> 60 & 0xF))) <<
        // 4;
        _jit_new_node_www(jit1, jit_code_rshi, _R10, _RAX, 4);
        _jit_new_node_www(jit1, jit_code_andi, _R13, _R10, 0xFF);
        //
        _jit_new_node_www(jit1, jit_code_andi, _R14, _RAX, 0xF);
        _jit_new_node_www(jit1, jit_code_lshi, _R14, _R14, 4);
        //
        _jit_new_node_www(jit1, jit_code_lroti, _R10, _RAX, 4);
        _jit_new_node_www(jit1, jit_code_andi, _R10, _R10, 0xF);
        _jit_new_node_www(jit1, jit_code_orr, _R10, _R14, _R10);
        //
        _jit_new_node_www(jit1, jit_code_xorr, _R10, _R13, _R10);
        _jit_new_node_www(jit1, jit_code_lshi, _R10, _R10, 4);
        _jit_new_node_www(jit1, jit_code_orr, _R11, _R11, _R10);

        // res |= (id << 44 & 0x0F00000000000000);
        _jit_new_node_www(jit1, jit_code_lshi, _R10, _RAX, 44);
        _jit_new_node_www(jit1, jit_code_andi, _R10, _R10, 0x0F00000000000000);
        _jit_new_node_www(jit1, jit_code_orr, _R11, _R11, _R10);

        // res |= (id >> 44 & 0x000000000000F000);
        _jit_new_node_www(jit1, jit_code_lroti, _R10, _RAX, 20);
        _jit_new_node_www(jit1, jit_code_andi, _R10, _R10, 0x000000000000F000);
        _jit_new_node_www(jit1, jit_code_orr, _R11, _R11, _R10);

        // ulong f = (id >> 16 ^ id >> 24) & 0xFF;
        // res |= ((f ^ id >> 24) & 0xFF) << 24 | f << 16;
        _jit_new_node_www(jit1, jit_code_rshi, _R13, _RAX, 16);
        _jit_new_node_www(jit1, jit_code_andi, _R13, _R13, 0xFF);
        _jit_new_node_www(jit1, jit_code_rshi, _R14, _RAX, 24);
        _jit_new_node_www(jit1, jit_code_andi, _R14, _R14, 0xFF);
        _jit_new_node_www(jit1, jit_code_xorr, _R10, _R13, _R14);
        //
        _jit_new_node_www(jit1, jit_code_rshi, _R13, _RAX, 24);
        _jit_new_node_www(jit1, jit_code_andi, _R13, _R13, 0xFF);
        _jit_new_node_www(jit1, jit_code_xorr, _R13, _R10, _R13);
        _jit_new_node_www(jit1, jit_code_lshi, _R13, _R13, 24);
        _jit_new_node_www(jit1, jit_code_lshi, _R10, _R10, 16);
        _jit_new_node_www(jit1, jit_code_orr, _R10, _R13, _R10);
        _jit_new_node_www(jit1, jit_code_orr, _R11, _R11, _R10);

        // res |= (id ^ 0x00FF00FF00000000) & 0x00FF00FF00000000;
        _jit_new_node_www(jit1, jit_code_xori, _R10, _RAX, 0x00FF00FF00000000);
        _jit_new_node_www(jit1, jit_code_andi, _R10, _R10, 0x00FF00FF00000000);
        _jit_new_node_www(jit1, jit_code_orr, _R11, _R11, _R10);

        _jit_retr(jit1, _R11, jit_code_retr_l); /*      retr   R2           */

        transform_id = (pulful)_jit_emit(jit1);
        _jit_clear_state(jit1);

        // pick encode
        _jit_prolog(jit2);
        _jit_getarg_l(jit2, _RAX, _jit_arg(jit2, jit_code_arg_l));
        // 1) <<1
        _jit_new_node_www(jit2, jit_code_lshi, _RAX, _RAX, 1);
        // 2) copy bit 1 and 2 to 2 and 3
        _jit_new_node_www(jit2, jit_code_andi, _R10, _RAX, 6);
        _jit_new_node_www(jit2, jit_code_ori, _RAX, _RAX, 12);
        _jit_new_node_www(jit2, jit_code_lshi, _R10, _R10, 1);
        _jit_new_node_www(jit2, jit_code_ori, _R10, _R10, 3);
        _jit_new_node_www(jit2, jit_code_andr, _RAX, _RAX, _R10);
        // 3) nand 2 and 3 and and with rest
        _jit_new_node_www(jit2, jit_code_andi, _R10, _RAX, 8);
        _jit_new_node_www(jit2, jit_code_andi, _R11, _RAX, 4);
        _jit_new_node_www(jit2, jit_code_lshi, _R11, _R11, 1);
        // for 0, nor
        _jit_new_node_www(jit2, jit_code_orr, _RBX, _R11, _R10);
        _jit_new_node_www(jit2, jit_code_xori, _RBX, _RBX, 8);
        _jit_new_node_www(jit2, jit_code_rroti, _RBX, _RBX, 3);
        // nand = and+xor
        _jit_new_node_www(jit2, jit_code_andr, _R11, _R10, _R11);
        _jit_new_node_www(jit2, jit_code_xori, _R11, _R11, 8);
        //
        _jit_new_node_www(jit2, jit_code_ori, _R11, _R11, 65527);
        _jit_new_node_www(jit2, jit_code_andr, _RAX, _RAX, _R11);
        _jit_new_node_www(jit2, jit_code_rshi, _R11, _R11, 1);
        _jit_new_node_www(jit2, jit_code_andr, _RAX, _RAX, _R11);
        _jit_new_node_www(jit2, jit_code_rshi, _R11, _R11, 1);
        _jit_new_node_www(jit2, jit_code_andr, _RAX, _RAX, _R11);
        _jit_new_node_www(jit2, jit_code_orr, _RAX, _RAX, _RBX);
        // set 7thbit
        _jit_new_node_www(jit2, jit_code_ori, _RAX, _RAX, 128);
        _jit_retr(jit2, _RAX, jit_code_retr_l);

        encode_pick = (psfs)_jit_emit(jit2);
        _jit_clear_state(jit2);

        // pick decode
        _jit_prolog(jit3);
        _jit_getarg_l(jit3, _RAX, _jit_arg(jit3, jit_code_arg_l));

        _jit_new_node_ww(jit3, jit_code_ctzr, _RAX, _RAX);
        _jit_new_node_ww(jit3, jit_code_popcntr, _RAX, _RAX);

        _jit_retr(jit3, _RAX, jit_code_retr_l);

        decode_pick = (psfs)_jit_emit(jit3);
        _jit_clear_state(jit3);
    }
    ~Overseer() {
        _jit_destroy_state(jit1);
        _jit_destroy_state(jit2);
        _jit_destroy_state(jit3);
        finish_jit();
    }

    template <typename T>
    DummyHead<T> *create_head(ChainOptions<T> options, bool is_obj) {
        DummyHead<T> *head = new DummyHead<T>();
        head->ttlu_max = options.max_time_til_update;
        head->times_updated = 0;
        head->is_obj = is_obj;
        head->max_len = options.max_chain_length > START_LEN
                            ? options.max_chain_length
                            : START_LEN;
        head->d = create_path<T>(START_LEN);
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
            if (!due_for_path_update.contains(head->d->id)) {
                due_for_path_update.push_back(head->d->id);
            }
        }
        check_and_update_head(head);
        return resolve_dummy(head->d);
    }
    template <typename T> T *resolve_head_val(DummyHead<T> *head) {
        return resolve_head(head)->tail->val;
    }

    // Could decide here whether to update chain or not based on knowledge of
    // change of value
    template <typename T> void change_value(DummyHead<T> *head, T val) {
        Dummy<T> *d = resolve_head(head);
        memcpy(d->tail->val, &val, sizeof(T));
    }

    void inc_time(long inc = 1) { time += inc; }
};

// TODO: Does this work properly?
inline Overseer *overseer = new Overseer();

template <typename T> class Protected {
  private:
    DummyHead<T> *head;
    T deobfuscate() const { return *overseer->resolve_head_val(head); }
    T *deobfuscate_ptr() const { return overseer->resolve_head_val(head); }
    void obfuscate(T val) { overseer->change_value(head, val); }

  public:
    // Protected() { head = overseer->create_head<T>(MAX_LEN, false); }
    // Protected(T val) {
    //     head = overseer->create_head<T>(MAX_LEN, false);
    //     overseer->change_value(head, val);
    // }
    Protected(ChainOptions<T> options = ChainOptions<T>()) {
        head = overseer->create_head<T>(options, false);
    }
    Protected(Protected<T> const &p) {
        head = overseer->create_head<T>(ChainOptions<T>(), false);
        obfuscate(p.val());
    }
    // TODO: LARS
    // Protected(Protected &&p) { printf("TODO MOVE CONSTRUCTOR\n"); }
    ~Protected() { overseer->free_head(head); }

    Protected &operator=(T val) {
        obfuscate(val);
        return *this;
    }
    Protected &operator=(Protected &val) {
        return operator=(val.deobfuscate());
    }
    // TODO:
    // Protected &operator=(Protected &&val) {
    //     printf("TODO MOVE assignment\n");
    //     return nullptr;
    // }
    // Protected operator+(Protected add) {
    //     T val = deobfuscate();
    //     T valadd = add.deobfuscate();
    //     Protected p(val + valadd);
    //     return p;
    // }
    // Protected operator+(T add) {
    //     T val = deobfuscate();
    //     Protected p(val + add);
    //     return p;
    // }
    Protected &operator+=(T add) {
        obfuscate(deobfuscate() + add);
        return *this;
    }
    Protected &operator++(int) { return this->operator+=(1); }
    Protected &operator-=(T add) { return this->operator+=(-add); }
    Protected &operator--(int) { return this->operator-=(1); }
    operator bool() const { return deobfuscate() != 0; }
    // Maybe this can work?
    // operator int() const { return deobfuscate(); }

    T val() { return deobfuscate(); }
    const T val() const { return deobfuscate(); }

    // NOTE: Only use this when you know what you are doing, as this could
    // induce undefined behavior
    T *ref() { return deobfuscate_ptr(); }
    T *operator&() { return ref(); }
    T *ref() const { return deobfuscate_ptr(); }
    T *operator&() const { return ref(); }

    operator T() { return deobfuscate(); }
};

// template <typename T> class ProtectedObj {
//   private:
//     DummyHead<T> *head;
//     T &deobfuscate() const { return *overseer->resolve_head_val(head); }
//     void obfuscate(T val) { overseer->change_value(head, val); }
//     void obfuscate(T *ptr) { overseer->change_value(head, *ptr); }
//
//   public:
//     ProtectedObj() { head = overseer->create_head<T>(MAX_LEN, true); }
//     ProtectedObj(T val) {
//         head = overseer->create_head<T>(MAX_LEN, true);
//         obfuscate(val);
//     }
//     ProtectedObj(T val, ushort max_len) {
//         head = overseer->create_head<T>(max_len, true);
//         obfuscate(val);
//     }
//     ProtectedObj(ProtectedObj const &p) {
//         head = overseer->create_head<T>(MAX_LEN, true);
//         change_value(head, p.val());
//     }
//     // TODO:
//     // ProtectedObj(ProtectedObj &&p) {
//     //     printf("move construct\n");
//     //     path = std::move(p.path);
//     // }
//     ~ProtectedObj() { overseer->free_head(head); }
//
//     ProtectedObj &operator=(T val) {
//         // TODO: This doesn't not work for objects
//         printf("LARS ERR\n");
//         obfuscate(val);
//         return *this;
//     }
//     ProtectedObj &operator=(T *val) {
//         obfuscate(val);
//         return *this;
//     }
//     ProtectedObj &operator=(ProtectedObj &val) {
//         return operator=(val.deobfuscate());
//     }
//     // TODO:
//     // ProtectedObj &operator=(ProtectedObj &&val) {
//     //     path = std::move(val.path);
//     //     printf("TODO MOVE assignment\n");
//     //     return *this;
//     // }
//     T &dot() { return deobfuscate(); }
//     const T &dot() const { return deobfuscate(); }
//     // TODO: test this
//     T &operator->() { return dot(); }
//
//     // ProtectedObj operator+(Protected add) {
//     //     T val = deobfuscate();
//     //     T valadd = add.deobfuscate();
//     //     ProtectedObj p(val + valadd);
//     //     return p;
//     // }
//     // ProtectedObj operator+(T add) {
//     //     T val = deobfuscate();
//     //     ProtectedObj p(val + add);
//     //     return p;
//     // }
//     // ProtectedObj &operator+=(T add) {
//     //     obfuscateAdd(add);
//     //     return *this;
//     // }
//     // ProtectedObj &operator++(int) { return this->operator+=(1); }
//     // ProtectedObj &operator-=(T add) { return this->operator+=(-add); }
//     // ProtectedObj &operator--(int) { return this->operator-=(1); }
//     // operator bool() const { return deobfuscate() != 0; }
//     // Maybe this can work?
//     // operator int() const { return deobfuscate(); }
//
//     T val() { return deobfuscate(); }
//     const T val() const { return deobfuscate(); }
//     void obfuscateAdd(T add) { obfuscate(deobfuscate() + add); }
// };
template <typename T> class Ptr {
  private:
    DummyHead<T> *head;
    T &deobfuscate() const { return *overseer->resolve_head_val(head); }
    T *deobfuscate_ptr() const { return overseer->resolve_head_val(head); }
    void obfuscate(T val) { overseer->change_value(head, val); }
    void obfuscate(T *ptr) { overseer->change_value(head, *ptr); }

  public:
    // Ptr() { head = overseer->create_head<T>(MAX_LEN, true); }
    Ptr(ChainOptions<T> options = ChainOptions<T>()) {
        head = overseer->create_head<T>(options, true);
    }
    Ptr(Ptr const &p) {
        head = overseer->create_head<T>(MAX_LEN, true);
        obfuscate(p.val());
    }
    // Ptr &operator=(Ptr &&) = delete;
    // TODO:
    // Ptr(Ptr &&p) {
    //     printf("move construct\n");
    //     path = std::move(p.path);
    // }
    ~Ptr() { overseer->free_head(head); }

    Ptr &operator=(T val) {
        // TODO: This doesn't not work for objects
        printf("LARS ERR\n");
        obfuscate(val);
        return *this;
    }
    Ptr &operator=(T *val) {
        obfuscate(val);
        return *this;
    }
    Ptr &operator=(Ptr &val) { return operator=(val.deobfuscate()); }
    // TODO:
    // Ptr &operator=(Ptr &&val) {
    //     path = std::move(val.path);
    //     printf("TODO MOVE assignment\n");
    //     return *this;
    // }
    // T &dot() { return deobfuscate(); }
    // const T &dot() const { return deobfuscate(); }
    T *operator->() { return deobfuscate_ptr(); }
    const T *operator->() const { return deobfuscate_ptr(); }

    T &operator*() { return deobfuscate(); }
    operator T *() { return deobfuscate_ptr(); }
};

class TEST {
  public:
    TEST() : val(2) {}
    int val = 0;
} typedef TEST;

class TEST2 {
  public:
    TEST2() {}
    int val = 0;

    operator int() { return val; }
    TEST2 &operator=(int val) {
        this->val = val;
        return *this;
    }
    TEST2 &operator=(TEST2 &val) { return operator=(val.val); }
} typedef TEST2;

// protectfunc inline int get_val_lars(ProtectedInt &val) { return val.val(); }
// template <typename T> inline void set_val_lars(Protected<T> &val, int set) {
//     printf("set_val_lars: %d, %d\n", val.val(), set);
//     val = set;
// }

// inline void add_val_lars(ProtectedInt &val, int add) {
//     printf("add_val_lars: %d, %d\n", val.val(), add);
//     val += add;
// }
////////////////////////////////////////////////////////////////////////////////////
