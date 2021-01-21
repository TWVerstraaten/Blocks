//
// Created by teunv on 1/21/2021.
//

#ifndef BLOCKS_STASH_H
#define BLOCKS_STASH_H

#include <cassert>

namespace str {

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

        explicit operator T*() {
            return m_value;
        }

      private:
        HAS_STASH m_hasStash = FALSE;

        T* m_value;
        T* m_stash;
    };

} // namespace str

#endif // BLOCKS_STASH_H
