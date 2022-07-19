/*  see copyright notice in squirrel.h */
#ifndef _SQSTD_BLOBIMPL_H_
#define _SQSTD_BLOBIMPL_H_

struct SQBlob : public SQStream
{
    SQBlob(isize_t size) {
        _size = size;
        _allocated = size;
        _buf = (unsigned char *)sq_malloc(size);
        memset(_buf, 0, _size);
        _ptr = 0;
        _owns = true;
    }
    virtual ~SQBlob() {
        sq_free(_buf, _allocated);
    }
    isize_t Write(void *buffer, isize_t size) {
        if(!CanAdvance(size)) {
            GrowBufOf(_ptr + size - _size);
        }
        memcpy(&_buf[_ptr], buffer, size);
        _ptr += size;
        return size;
    }
    isize_t Read(void *buffer,isize_t size) {
        isize_t n = size;
        if(!CanAdvance(size)) {
            if((_size - _ptr) > 0)
                n = _size - _ptr;
            else return 0;
        }
        memcpy(buffer, &_buf[_ptr], n);
        _ptr += n;
        return n;
    }
    bool Resize(isize_t n) {
        if(!_owns) return false;
        if(n != _allocated) {
            unsigned char *newbuf = (unsigned char *)sq_malloc(n);
            memset(newbuf,0,n);
            if(_size > n)
                memcpy(newbuf,_buf,n);
            else
                memcpy(newbuf,_buf,_size);
            sq_free(_buf,_allocated);
            _buf=newbuf;
            _allocated = n;
            if(_size > _allocated)
                _size = _allocated;
            if(_ptr > _allocated)
                _ptr = _allocated;
        }
        return true;
    }
    bool GrowBufOf(isize_t n)
    {
        bool ret = true;
        if(_size + n > _allocated) {
            if(_size + n > _size * 2)
                ret = Resize(_size + n);
            else
                ret = Resize(_size * 2);
        }
        _size = _size + n;
        return ret;
    }
    bool CanAdvance(isize_t n) {
        if(_ptr+n>_size)return false;
        return true;
    }
    isize_t Seek(isize_t offset, isize_t origin) {
        switch(origin) {
            case SQ_SEEK_SET:
                if(offset > _size || offset < 0) return -1;
                _ptr = offset;
                break;
            case SQ_SEEK_CUR:
                if(_ptr + offset > _size || _ptr + offset < 0) return -1;
                _ptr += offset;
                break;
            case SQ_SEEK_END:
                if(_size + offset > _size || _size + offset < 0) return -1;
                _ptr = _size + offset;
                break;
            default: return -1;
        }
        return 0;
    }
    bool IsValid() {
        return _size == 0 || _buf?true:false;
    }
    bool EOS() {
        return _ptr == _size;
    }
    isize_t Flush() { return 0; }
    isize_t Tell() { return _ptr; }
    isize_t Len() { return _size; }
    PVOID GetBuf(){ return _buf; }
private:
    isize_t _size;
    isize_t _allocated;
    isize_t _ptr;
    unsigned char *_buf;
    bool _owns;
};

#endif //_SQSTD_BLOBIMPL_H_
