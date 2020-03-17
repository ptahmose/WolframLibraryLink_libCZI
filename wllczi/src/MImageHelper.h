#pragma once

#include "inc_libCzi.h"
#include <WolframLibrary.h>
#include <WolframImageLibrary.h>
#include <cstdint>

class MImageHelper
{
public:
    static MImage CreateMImage(WolframImageLibrary_Functions imgLibFunctions, const libCZI::IntSize& size, libCZI::PixelType pixeltype);
    static bool TryGetPixelType(WolframImageLibrary_Functions imgLibFunctions, MImage mimg, libCZI::PixelType* pixelType);
    static int GetBytesPerPel(libCZI::PixelType pixelType);

    /// If the specified bitmap is of type RGB24 or RGB48, then they will be converted (in-place) to BGR24 or BGR48.
    /// For other pixel types, this is a no-op.
    /// \param [in] bd The bitmap.
    static void SwapRgb(libCZI::IBitmapData* bd);

    static void ConvertInplace_RGB24_to_BGR24(std::uint32_t w, std::uint32_t h, uint32_t stride, void* ptr);
    static void ConvertInplace_RGB48_to_BGR48(std::uint32_t w, std::uint32_t h, uint32_t stride, void* ptr);
};


class CMImageWrapper : public libCZI::IBitmapData
{
private:
    MImage mimg;
    WolframImageLibrary_Functions imgLibFunctions;
    libCZI::PixelType pixelType;
public:
    CMImageWrapper(WolframImageLibrary_Functions imgLibFunctions, MImage mimg) : mimg(mimg), imgLibFunctions(imgLibFunctions)
    {
        MImageHelper::TryGetPixelType(imgLibFunctions, mimg, &this->pixelType);
    }

    /// Gets pixel type.
    ///
    /// \return The pixel type.
    virtual libCZI::PixelType GetPixelType() const
    {
        return this->pixelType;
    }

    /// Gets the size of the bitmap (i. e. its width and height in pixels).
    ///
    /// \return The size (in pixels).
    virtual libCZI::IntSize GetSize() const
    {
        return libCZI::IntSize{ (std::uint32_t)this->imgLibFunctions->MImage_getColumnCount(this->mimg),(std::uint32_t)this->imgLibFunctions->MImage_getRowCount(this->mimg) };
    }

    /// Gets a data structure allowing for direct access of the bitmap.
    /// 
    /// The BitmapLockInfo returned must only considered to be valid until Unlock is called.
    /// It is legal to call Lock multiple time (also from different threads concurrently).
    /// In any case, calls to Lock and Unlock must be balanced. It is considered to be a
    /// fatal error if the object is destroyed when it is locked.
    ///
    /// \return The BitmapLockInfo allowing to directly access the data representing the bitmap.
    virtual libCZI::BitmapLockInfo Lock()
    {
        libCZI::BitmapLockInfo lockInfo;
        lockInfo.ptrData = lockInfo.ptrDataRoi = this->imgLibFunctions->MImage_getRawData(this->mimg);
        lockInfo.stride = MImageHelper::GetBytesPerPel(this->pixelType) * this->imgLibFunctions->MImage_getColumnCount(this->mimg);
        lockInfo.size = ((std::uint64_t)lockInfo.stride) * this->imgLibFunctions->MImage_getRowCount(this->mimg);
        return lockInfo;
    }

    /// Inform the bitmap object that the data (previously retrieved by a call to Lock)
    /// is not longer used.
    /// 
    /// The BitmapLockInfo returned must only considered to be valid until Unlock is called.
    virtual void Unlock()
    {}
};