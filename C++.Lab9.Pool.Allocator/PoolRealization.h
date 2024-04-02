template <typename value_type>
class Pool {
public:
    value_type* pool_allocate(int size){

        free_cell = static_cast<value_type*>(new value_type[size]);

        return free_cell;
    }
private:
    value_type *free_cell;
};


template <typename T, size_t PoolSize>
class PoolAllocator {
public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T*;



    template<class Other>
    struct rebind {
        typedef PoolAllocator<Other, PoolSize> other;
    };


    PoolAllocator():
    _pool(nullptr),
    _free_cell(nullptr),
    _end(nullptr),
    new_pool()
    {}

    ~PoolAllocator(){
        destroy();
    }

    template <typename U>
    PoolAllocator(const PoolAllocator<U, PoolSize>&) : _pool(nullptr), _free_cell(nullptr), _end(nullptr), new_pool() {}

    void destroy(pointer p) {
        p->~value_type();
    }

    pointer allocate(size_type n = 1) {
        //eсли пул еще не создан
        if (_pool == nullptr) {
            _pool = new_pool.pool_allocate(PoolSize);
            //_pool = static_cast<pointer>(new value_type[PoolSize]);
            _free_cell = _pool; //началo пула
            _end = _free_cell + PoolSize; //конец пула
        }

        //если нет мест в пуле, создадим новый пул
        if (_free_cell + n > _end){
            _pool = new_pool.pool_allocate(PoolSize);
            //_pool = static_cast<pointer>(new value_type[PoolSize]);
            _free_cell = _pool;
            _end = _free_cell + PoolSize;
        }

        //указатель на следующую свободную ячейку
        pointer result = _free_cell;
        _free_cell+=n;
        return result;
    }

    size_type max_size() noexcept {
        return PoolSize;
    }

    void deallocate(pointer p, size_type i) {
        if (p != nullptr) {
            _end--;
            std::swap(*p, *_end);
        }
    }

    void destroy(){
        if (_pool != nullptr) {//если пул существует
            delete [] _pool;
            _pool = nullptr;
            _free_cell = nullptr;
            _end = nullptr;
        }
    }

private:
    pointer _pool;
    pointer _free_cell;
    pointer _end;
    Pool<value_type> new_pool;
};

