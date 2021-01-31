//
// Created by teunv on 1/21/2021.
//

#ifndef BLOCKS_STASH_H
#define BLOCKS_STASH_H

#include <cassert>

namespace datstr {

    template <typename T>
    class Stash {

      public:
        explicit Stash(T* value) : m_value(value), m_stash(nullptr) {
        }

        ~Stash() {
            if (m_hasStash == true) {
                assert(m_stash != nullptr);
                delete m_stash;
                m_stash = nullptr;
            } else {
                assert(m_stash == nullptr);
            }
        }

        void stash() {
            assert(m_hasStash == false);
            m_hasStash = true;
            std::swap(m_value, m_stash);
        }

        T* get() {
            return m_value;
        }

        T* get() const {
            return m_value;
        }

        void set(T* value) {
            m_value = value;
        }

        void recover() {
            assert(m_hasStash == true);
            m_hasStash = false;
            std::swap(m_value, m_stash);
            delete m_stash;
            m_stash = nullptr;
        }

        T& operator*() {
            return *m_value;
        }

        T* operator->() {
            return m_value;
        }

        explicit operator T*() {
            return m_value;
        }

        T* stashedValue() {
            assert(m_hasStash == true);
            return m_stash;
        }

      private:
        bool m_hasStash = false;

        T* m_value;
        T* m_stash;
    };

} // namespace datstr

#endif // BLOCKS_STASH_H
