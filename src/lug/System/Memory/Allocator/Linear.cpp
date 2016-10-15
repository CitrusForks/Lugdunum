#include <lug/System/Memory/Allocator/Linear.hpp>
#include <cstdlib>
#include <memory>
#include <lug/System/Debug.hpp>

lug::System::Memory::Allocator::Linear::Linear(lug::System::Memory::Area::IArea* area) : _area{area}, _currentPage{_area->requestNextPage()}, _firstPage{_currentPage} {
    if (_currentPage) {
        _current = _currentPage->start;
    }
}

void* lug::System::Memory::Allocator::Linear::allocate(size_t size, size_t alignment, size_t offset) {
    LUG_ASSERT(size > offset, "The size must be greater than the offset");

    // Adapt the size to store the size at the beginning of the block
    const size_t newSize = size + sizeof(size_t);
    const size_t newOffset = offset + sizeof(size_t);

    while (_current) {
        // Try to allocate memory on the current page
        _current = static_cast<char*>(_current) + newOffset;
        if (_current <= _currentPage->end) {
            size_t sizeLeft = static_cast<char*>(_currentPage->end) - static_cast<char*>(_current) + 1;
            if (std::align(alignment, newSize - newOffset, _current, sizeLeft)) {
                _current = static_cast<char*>(_current) + newSize - newOffset;

                // Store the size
                *static_cast<size_t*>(static_cast<void*>(static_cast<char*>(_current) - newSize)) = size;

                return static_cast<char*>(_current) - size;
            }
        }

        // Out of memory on this page, just request a new one and try to reallocate
        _currentPage = _area->requestNextPage();
        _current = _currentPage ? _currentPage->start : nullptr;
    }

    return nullptr;
}

void lug::System::Memory::Allocator::Linear::free(void*) const {
    // Do nothing here
}

void lug::System::Memory::Allocator::Linear::reset() {
    _currentPage = _firstPage;

    if (_currentPage) {
        _current = _currentPage->start;
    }
}

size_t lug::System::Memory::Allocator::Linear::getSize(void *ptr) const {
    return static_cast<size_t*>(ptr)[-1];
}
