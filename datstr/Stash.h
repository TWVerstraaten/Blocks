//
// Created by teunv on 1/21/2021.
//

#ifndef BLOCKS_STASH_H
#define BLOCKS_STASH_H

#include <cassert>

namespace datstr {

    template <typename T>
    class Stash {

        enum HAS_STASH { TRUE, FALSE };

      public:
        explicit Stash(T* value) : m_value(value), m_stash(nullptr) {
        }

        ~Stash() {
            if (m_hasStash == TRUE) {
                assert(m_stash != nullptr);
                delete m_stash;
                m_stash = nullptr;
            } else {
                assert(m_stash == nullptr);
            }
        }

        void stash() {
            assert(m_hasStash == FALSE);
            m_hasStash = TRUE;
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
            assert(m_hasStash == TRUE);
            m_hasStash = FALSE;
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
            assert(m_hasStash == TRUE);
            return m_stash;
        }

      private:
        HAS_STASH m_hasStash = FALSE;

        T* m_value;
        T* m_stash;
    };

} // namespace datstr

#endif // BLOCKS_STASH_H
