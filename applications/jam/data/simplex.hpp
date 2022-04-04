#pragma once
#include "../engine_include.hpp"

using namespace lgn;

struct simplex
{
    friend struct simplex_iterator;
private:
    std::array<math::vec3, 4> m_data;
    size_type m_size = 0;
    size_type m_front = 0;

public:
    using iterator = struct simplex_iterator;
    using const_iterator = struct const_simplex_iterator;

    constexpr void push_front(const math::vec3& point) noexcept
    {
        m_size++;
        m_front = index_at(3);
        m_data[m_front] = point;
    }

    constexpr void swap(size_type i, size_type j) noexcept
    {
        auto& lhs = at(i);
        auto& rhs = at(j);
        math::vec3 tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }

    constexpr void erase(size_type i) noexcept
    {
        if (m_size == 0)
            return;

        if (i != m_size - 1)
            swap(i, m_size - 1);

        m_size--;
    }

    bool next(math::vec3& direction) noexcept
    {
        switch (m_size)
        {
        case 2:
            return lineCase(direction);
        case 3:
            return triangleCase(direction);
        case 4:
            return tetrahedronCase(direction);
        default:
            return false;
        }
    }

    constexpr L_ALWAYS_INLINE size_type size() const noexcept { return m_size; }

    constexpr L_ALWAYS_INLINE math::vec3& at(size_type i) noexcept { return m_data[index_at(i)]; }
    constexpr L_ALWAYS_INLINE const math::vec3& at(size_type i) const noexcept { return m_data[index_at(i)]; }

    constexpr L_ALWAYS_INLINE math::vec3& operator[](size_type i) noexcept { return m_data[index_at(i)]; }
    constexpr L_ALWAYS_INLINE const math::vec3& operator[](size_type i) const noexcept { return m_data[index_at(i)]; }

    constexpr L_ALWAYS_INLINE iterator begin() noexcept;
    constexpr L_ALWAYS_INLINE const_iterator begin() const noexcept;
    constexpr L_ALWAYS_INLINE iterator end() noexcept;
    constexpr L_ALWAYS_INLINE const_iterator end() const noexcept;

private:

    constexpr L_ALWAYS_INLINE size_type index_at(size_type i) const noexcept
    {
        return (m_front + i) % 4;
    }

    bool lineCase(math::vec3& direction) noexcept
    {
        auto& first = at(0);
        auto& second = at(1);

        auto firstToSecond = second - first;
        auto toOrigin = -first;

        if (math::dot(firstToSecond, toOrigin) > 0.f)
            direction = math::normalize(math::cross(math::cross(firstToSecond, toOrigin), firstToSecond));
        else
        {
            erase(1);
            direction = math::normalize(toOrigin);
        }

        return false;
    }

    bool triangleCase(math::vec3& direction) noexcept
    {
        auto& first = at(0);
        auto& second = at(1);
        auto& third = at(2);

        auto firstToSecond = second - first;
        auto firstToThird = third - first;
        auto toOrigin = -first;

        auto trianglePerp = math::cross(firstToSecond, firstToThird);

        if (math::dot(math::cross(trianglePerp, firstToThird), toOrigin) > 0.f)
        {
            if (math::dot(firstToThird, toOrigin) > 0.f)
            {
                erase(1);
                direction = math::normalize(math::cross(math::cross(firstToThird, toOrigin), firstToThird));
            }
            else
            {
                erase(2);
                return lineCase(direction);
            }
        }
        else
        {
            if (math::dot(math::cross(firstToSecond, trianglePerp), toOrigin) > 0.f)
            {
                erase(2);
                return lineCase(direction);
            }
            else
            {
                if (math::dot(trianglePerp, toOrigin) > 0.f)
                {
                    direction = math::normalize(trianglePerp);
                }
                else
                {
                    swap(1, 2);
                    direction = math::normalize(-trianglePerp);
                }
            }
        }

        return false;
    }

    bool tetrahedronCase(math::vec3& direction) noexcept
    {
        auto& first = at(0);
        auto& second = at(1);
        auto& third = at(2);

        auto firstToSecond = second - first;
        auto firstToThird = third - first;
        auto toOrigin = -first;

        auto trianglePerp = math::cross(firstToSecond, firstToThird);

        if (math::dot(trianglePerp, toOrigin) > 0.f)
        {
            erase(3);
            return triangleCase(direction);
        }

        auto& fourth = at(3);
        auto firstToFourth = fourth - first;
        trianglePerp = math::cross(firstToThird, firstToFourth);
        if (math::dot(trianglePerp, toOrigin) > 0.f)
        {
            swap(1, 2);
            erase(2);
            return triangleCase(direction);
        }

        trianglePerp = math::cross(firstToFourth, firstToSecond);
        if (math::dot(trianglePerp, toOrigin) > 0.f)
        {
            erase(1);
            return triangleCase(direction);
        }

        return true;
    }
};

struct [[no_reflect]] const_simplex_iterator
{
    using value_type = math::vec3;
    using difference_type = ptrdiff_t;
    using pointer = const math::vec3*;
    using reference = const math::vec3&;
    using iterator_category = std::random_access_iterator_tag;
private:
    const simplex* m_ptr;
    size_type m_idx;

public:
    constexpr L_ALWAYS_INLINE const_simplex_iterator() noexcept : m_ptr(nullptr), m_idx(0) {}

    constexpr L_ALWAYS_INLINE explicit const_simplex_iterator(const simplex* ptr, size_t idx = 0) noexcept : m_ptr(ptr), m_idx(idx) {}

    L_NODISCARD constexpr L_ALWAYS_INLINE reference operator*() const noexcept { return m_ptr->at(m_idx); }
    L_NODISCARD constexpr L_ALWAYS_INLINE pointer operator->() const noexcept { return &m_ptr->at(m_idx); }

    constexpr L_ALWAYS_INLINE const_simplex_iterator& operator++() noexcept { m_idx++; return *this; }
    constexpr L_ALWAYS_INLINE const_simplex_iterator operator++(int) noexcept { const_simplex_iterator tmp = *this; m_idx++; return tmp; }

    constexpr L_ALWAYS_INLINE const_simplex_iterator& operator--() noexcept { m_idx--; return *this; }

    constexpr L_ALWAYS_INLINE const_simplex_iterator operator--(int) noexcept { const_simplex_iterator tmp = *this; m_idx--; return tmp; }

    constexpr L_ALWAYS_INLINE const_simplex_iterator& operator+=(const difference_type offset) noexcept { m_idx += offset; return *this; }
    L_NODISCARD constexpr L_ALWAYS_INLINE const_simplex_iterator operator+(const difference_type offset) const noexcept { return const_simplex_iterator(m_ptr, m_idx + offset); }

    constexpr L_ALWAYS_INLINE const_simplex_iterator& operator-=(const difference_type offset) noexcept { m_idx -= offset; return *this; }
    L_NODISCARD constexpr L_ALWAYS_INLINE const_simplex_iterator operator-(const difference_type offset) const noexcept { return const_simplex_iterator(m_ptr, m_idx - offset); }
    L_NODISCARD constexpr L_ALWAYS_INLINE difference_type operator-(const const_simplex_iterator& other) const noexcept { return m_idx - other.m_idx; }

    L_NODISCARD constexpr L_ALWAYS_INLINE reference operator[](const difference_type offset) const noexcept { return m_ptr->at(m_idx + offset); }

    L_NODISCARD constexpr L_ALWAYS_INLINE bool operator==(const const_simplex_iterator& other) const noexcept { return (m_idx == other.m_idx) && (m_ptr == other.m_ptr); }
    L_NODISCARD constexpr L_ALWAYS_INLINE bool operator!=(const const_simplex_iterator& other) const noexcept { return !(*this == other); }
};

struct [[no_reflect]] simplex_iterator : public const_simplex_iterator
{
    using value_type = math::vec3;
    using difference_type = ptrdiff_t;
    using pointer = math::vec3*;
    using reference = math::vec3&;
    using iterator_category = std::random_access_iterator_tag;

    constexpr L_ALWAYS_INLINE simplex_iterator() noexcept : const_simplex_iterator() {}

    constexpr L_ALWAYS_INLINE explicit simplex_iterator(simplex* ptr, size_t idx = 0) noexcept : const_simplex_iterator(ptr, idx) {}

    L_NODISCARD constexpr L_ALWAYS_INLINE reference operator*() const noexcept { return const_cast<reference>(const_simplex_iterator::operator*()); }
    L_NODISCARD constexpr L_ALWAYS_INLINE pointer operator->() const noexcept { return const_cast<pointer>(const_simplex_iterator::operator->()); }

    constexpr L_ALWAYS_INLINE simplex_iterator& operator++() noexcept { const_simplex_iterator::operator++(); return *this; }
    constexpr L_ALWAYS_INLINE simplex_iterator operator++(int) noexcept { simplex_iterator tmp = *this; const_simplex_iterator::operator++(); return tmp; }

    constexpr L_ALWAYS_INLINE simplex_iterator& operator--() noexcept { const_simplex_iterator::operator--(); return *this; }
    constexpr L_ALWAYS_INLINE simplex_iterator operator--(int) noexcept { simplex_iterator tmp = *this; const_simplex_iterator::operator--(); return tmp; }

    constexpr L_ALWAYS_INLINE simplex_iterator& operator+=(const ptrdiff_t offset) noexcept { const_simplex_iterator::operator+=(offset); return *this; }
    L_NODISCARD constexpr L_ALWAYS_INLINE simplex_iterator operator+(const ptrdiff_t offset) const noexcept { simplex_iterator tmp = *this; tmp += offset; return tmp; }

    constexpr L_ALWAYS_INLINE simplex_iterator& operator-=(const ptrdiff_t offset) noexcept { const_simplex_iterator::operator-=(offset); return *this; }
    L_NODISCARD constexpr L_ALWAYS_INLINE simplex_iterator operator-(const ptrdiff_t offset) const noexcept { simplex_iterator tmp = *this; tmp -= offset; return tmp; }
    L_NODISCARD constexpr L_ALWAYS_INLINE difference_type operator-(const const_simplex_iterator& other) const noexcept { return const_simplex_iterator::operator-(other); }

    L_NODISCARD constexpr L_ALWAYS_INLINE reference operator[](const ptrdiff_t offset) const noexcept { return const_cast<reference>(const_simplex_iterator::operator[](offset)); }

    L_NODISCARD constexpr L_ALWAYS_INLINE bool operator==(const const_simplex_iterator& other) const noexcept { return const_simplex_iterator::operator==(other); }
    L_NODISCARD constexpr L_ALWAYS_INLINE bool operator!=(const const_simplex_iterator& other) const noexcept { return const_simplex_iterator::operator!=(other); }
};

constexpr L_ALWAYS_INLINE typename simplex::iterator simplex::begin() noexcept { return iterator(this, 0); }
constexpr L_ALWAYS_INLINE typename simplex::const_iterator simplex::begin() const noexcept { return const_iterator(this, 0); }
constexpr L_ALWAYS_INLINE typename simplex::iterator simplex::end() noexcept { return iterator(this, m_size); }
constexpr L_ALWAYS_INLINE typename simplex::const_iterator simplex::end() const noexcept { return const_iterator(this, m_size); }
