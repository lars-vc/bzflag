#pragma once
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#define protectfunc /*PROTECTOR_FUNCTION_PRIO=5*/
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

template <typename T> struct DummyTail {
    T *val;
};

template <typename T> struct Dummy {
    int id;
    Dummy<T> *d1;
    Dummy<T> *d2;
    Dummy<T> *d3;
    DummyTail<T> *tail;
};
class Overseer {
  private:
    // idk i need this function without the if check cus otherwise we cause inf
    // recursion but I also want to keep the other function
    template <typename T> DummyTail<T> *resolve_dummy_interal(Dummy<T> *d) {
        int pick = catalog[d->id];
        Dummy<T> *next = d;
        while (pick != 4) {
            switch (pick) {
            case 0:
                next = next->d1;
                break;
            case 1:
                next = next->d2;
                break;
            case 2:
                next = next->d3;
                break;
            }
            pick = catalog[next->id];
        }
        return next->tail;
    }
    template <typename T> Dummy<T> *create_path_rec(int depth, int max_depth) {
        int c = totalcounter;
        totalcounter++;
        Dummy<T> *d = new Dummy<T>();
        if (depth < max_depth) {
            d->d1 = create_path_rec<T>(depth + 1, max_depth);
            d->d2 = create_path_rec<T>(depth + 1, max_depth);
            d->d3 = create_path_rec<T>(depth + 1, max_depth);
        }
        // d->tail = new DummyTail<T>();
        d->tail = (DummyTail<T> *)malloc(sizeof(DummyTail<T>));
        d->tail->val = (T *)malloc(sizeof(T));
        // d->tail->val = nullptr;
        // TODO: LARS
        // d->id = catalog.size();
        d->id = c;
        // d->id = rand();
        counter[d->id] = 0;
        catalog[d->id] = -1;
        // d->tail->val = rand();
        return d;
    }

    template <typename T> void free_path_rec(Dummy<T> *d) {
        if (d->d1 != nullptr) {
            free_path_rec(d->d1);
            free_path_rec(d->d2);
            free_path_rec(d->d3);
        }
        catalog.erase(d->id);
        free(d->tail->val);
        free(d->tail);
        // TODO: fix warning
        delete d;
    }

    template <typename T> void check_and_update_dummy(Dummy<T> *d) {
        int index = -1;
        for (int i : due_for_path_update) {
            if (i == d->id) {
                index = i;
            }
        }
        if (index != -1) {
            update_path_dummy(d, resolve_dummy_interal(d)->val);
            due_for_path_update.erase(std::remove(due_for_path_update.begin(),
                                                  due_for_path_update.end(),
                                                  d->id),
                                      due_for_path_update.end());
            counter[d->id] = 0;
            // due_for_path_update.erase(due_for_path_update.begin() + index);
            // printf("Updated path\n");
            // for (const auto &elem : catalog) { std::cout << elem.first << ","
            // << elem.second << " ";
            // }
            // printf("\n");
        }
    }

    // id to correct path
    std::map<int, int> catalog;
    // id to amount of times updated
    std::map<int, int> counter;
    // update path on next resolve
    std::vector<int> due_for_path_update;
    int paths_created = 0;
    int totalcounter = 0;

  public:
    template <typename T> Dummy<T> *create_path(T val) {
        // int len = rand();
        int len = 10;
        Dummy<T> *d = create_path_rec<T>(0, len);
        T *valptr = (T *)malloc(sizeof(T));
        memcpy(valptr, &val, sizeof(T));
        update_path_dummy(d, valptr);
        paths_created++;
        printf("Created path: id=%d name=%s count=%d\n", d->id,
               typeid(val).name(), paths_created);
        return d;
    }

    template <typename T> Dummy<T> *create_path_obj() {
        // int len = rand();
        int len = 10;
        Dummy<T> *d = create_path_rec<T>(0, len);
        // T *valptr = (T *)malloc(sizeof(T));
        T *valptr = new T();
        update_path_dummy(d, valptr);
        paths_created++;
        printf("Created path: id=%d name=%s count=%d\n", d->id,
               typeid(*valptr).name(), paths_created);
        return d;
    }

    template <typename T> void free_path(Dummy<T> *d) {
        printf("Freed:        id=%d\n", d->id);
        free_path_rec(d);
    }

    template <typename T> DummyTail<T> *resolve_dummy(Dummy<T> *d) {
        // check if dummy is due for chain update
        check_and_update_dummy(d);

        int pick = catalog[d->id];
        Dummy<T> *next = d;
        while (pick != 4) {
            switch (pick) {
            case 0:
                next = next->d1;
                break;
            case 1:
                next = next->d2;
                break;
            case 2:
                next = next->d3;
                break;
            }
            pick = catalog[next->id];
        }
        return next->tail;
    }

    template <typename T> T *resolve_dummy_val(Dummy<T> *d) {
        return resolve_dummy(d)->val;
    }

    template <typename T> void update_path_dummy(Dummy<T> *d) {
        T *val = resolve_dummy_val(d);
        update_path_dummy(d, val);
    }

    template <typename T> void update_path_dummy(Dummy<T> *d, T *val) {
        Dummy<T> *curr = d;
        while (curr->d1 != nullptr) {
            int pick = rand() % 3;
            catalog[curr->id] = pick;
            switch (pick) {
            case 0:
                curr = curr->d1;
                break;
            case 1:
                curr = curr->d2;
                break;
            case 2:
                curr = curr->d3;
                break;
            }
        }
        catalog[curr->id] = 4;
        // printf("memcpy\n");
        memcpy(curr->tail->val, val, sizeof(T));
        // curr->tail->val = val;
    }

    // Could decide here whether to update chain or not based on knowledge of
    // change of value
    template <typename T> void change_value_dummy(Dummy<T> *d, T val) {
        // TODO: When to update?
        counter[d->id]++;

        if (counter[d->id] > 1000 &&
            std::find(due_for_path_update.begin(), due_for_path_update.end(),
                      d->id) == due_for_path_update.end()) {
            printf("DUE\n");
            due_for_path_update.push_back(d->id);
        }
        *resolve_dummy(d)->val = val;
        // memcpy(resolve_dummy(d)->val, &val, sizeof(T));
    }
};

// TODO: Does this work properly?
inline Overseer *overseer = new Overseer();

template <typename T> class Protected {
  private:
    Dummy<T> *path;
    T deobfuscate() const { return *overseer->resolve_dummy_val(path); }
    void obfuscate(T val) { overseer->change_value_dummy(path, val); }

  public:
    Protected() { path = overseer->create_path(static_cast<T>(0)); }
    Protected(T val) { path = overseer->create_path(val); }
    Protected(Protected const &p) { path = overseer->create_path(p.val()); }
    // TODO: LARS
    // Protected(Protected &&p) { printf("TODO MOVE CONSTRUCTOR\n"); }
    ~Protected() {
        // printf("freed\n");
        overseer->free_path(path);
    }

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
    Protected operator+(Protected add) {
        T val = deobfuscate();
        T valadd = add.deobfuscate();
        Protected p(val + valadd);
        return p;
    }
    Protected operator+(T add) {
        T val = deobfuscate();
        Protected p(val + add);
        return p;
    }
    Protected &operator+=(T add) {
        obfuscateAdd(add);
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
    void obfuscateAdd(T add) { obfuscate(deobfuscate() + add); }
};

template <typename T> class ProtectedObj {
  private:
    Dummy<T> *path;
    T &deobfuscate() const { return *overseer->resolve_dummy_val(path); }
    void obfuscate(T val) { overseer->change_value_dummy(path, val); }

  public:
    ProtectedObj() { path = overseer->create_path_obj<T>(); }
    // ProtectedObj(T val) { path = overseer->create_path(val); }
    ProtectedObj(ProtectedObj const &p) {
        path = overseer->create_path(p.val());
    }
    // TODO:
    // ProtectedObj(ProtectedObj &&p) {
    //     printf("move construct\n");
    //     path = std::move(p.path);
    // }
    ~ProtectedObj() {
        // printf("freedobj\n");
        overseer->free_path(path);
    }

    ProtectedObj &operator=(T val) {
        // TODO: This doesn't not work for objects
        printf("LARS ERR\n");
        obfuscate(val);
        return *this;
    }
    ProtectedObj &operator=(ProtectedObj &val) {
        return operator=(val.deobfuscate());
    }
    // TODO:
    // ProtectedObj &operator=(ProtectedObj &&val) {
    //     path = std::move(val.path);
    //     printf("TODO MOVE assignment\n");
    //     return *this;
    // }
    T &dot() {
        // overseer->update_path_dummy(path);
        return deobfuscate();
    }
    const T &dot() const {
        // overseer->update_path_dummy(path);
        return deobfuscate();
    }
    // TODO: test this
    T &operator->() { return dot(); }

    // ProtectedObj operator+(Protected add) {
    //     T val = deobfuscate();
    //     T valadd = add.deobfuscate();
    //     ProtectedObj p(val + valadd);
    //     return p;
    // }
    // ProtectedObj operator+(T add) {
    //     T val = deobfuscate();
    //     ProtectedObj p(val + add);
    //     return p;
    // }
    // ProtectedObj &operator+=(T add) {
    //     obfuscateAdd(add);
    //     return *this;
    // }
    // ProtectedObj &operator++(int) { return this->operator+=(1); }
    // ProtectedObj &operator-=(T add) { return this->operator+=(-add); }
    // ProtectedObj &operator--(int) { return this->operator-=(1); }
    // operator bool() const { return deobfuscate() != 0; }
    // Maybe this can work?
    // operator int() const { return deobfuscate(); }

    T val() { return deobfuscate(); }
    const T val() const { return deobfuscate(); }
    void obfuscateAdd(T add) { obfuscate(deobfuscate() + add); }
};

class TEST {
  public:
    TEST() : val(2) {}
    Protected<int> val = 0;
} typedef TEST;

// protectfunc inline int get_val_lars(ProtectedInt &val) { return val.val(); }
// inline void set_val_lars(ProtectedInt &val, int set) {
//     printf("set_val_lars: %d, %d\n", val.val(), set);
//     val = set;
// }

// inline void add_val_lars(ProtectedInt &val, int add) {
//     printf("add_val_lars: %d, %d\n", val.val(), add);
//     val += add;
// }
