#include "MImageHelper.h"
#include <WolframImageLibrary.h>

using namespace libCZI;

/*static*/int MImageHelper::GetBytesPerPel(libCZI::PixelType pixelType)
{
    switch (pixelType)
    {
        case PixelType::Gray8:              return 1;
        case PixelType::Gray16:             return 2;
        case PixelType::Gray32Float:        return 4;
        case PixelType::Bgr24:              return 3;
        case PixelType::Bgr48:              return 6;
        case PixelType::Bgr96Float:         return 12;
        case PixelType::Bgra32:             return 4;
        case PixelType::Gray64ComplexFloat: return 16;
        case PixelType::Bgr192ComplexFloat: return 48;
        case PixelType::Gray32:             return 4;
        case PixelType::Gray64Float:        return 8;
        default:
            throw std::invalid_argument("illegal pixeltype");
    }
}

/*static*/MImage MImageHelper::CreateMImage(WolframImageLibrary_Functions imgLibFunctions, const libCZI::IntSize& size, libCZI::PixelType pixeltype)
{
    MImage mimg = nullptr;;
    int r;
    switch (pixeltype)
    {
        case PixelType::Gray8:
            r = (imgLibFunctions->MImage_new2D)(size.w, size.h, 1, MImage_Type_Bit8, MImage_CS_Gray, False, &mimg);
            break;
        case PixelType::Gray16:
            r = (imgLibFunctions->MImage_new2D)(size.w, size.h, 1, MImage_Type_Bit16, MImage_CS_Gray, False, &mimg);
            break;
        case PixelType::Bgr24:
            r = (imgLibFunctions->MImage_new2D)(size.w, size.h, 3, MImage_Type_Bit8, MImage_CS_RGB, True, &mimg);
            break;
        case PixelType::Bgr48:
            r = (imgLibFunctions->MImage_new2D)(size.w, size.h, 3, MImage_Type_Bit16, MImage_CS_RGB, True, &mimg);
            break;
        case PixelType::Gray32Float:
            r = (imgLibFunctions->MImage_new2D)(size.w, size.h, 1, MImage_Type_Real32, MImage_CS_Gray, False, &mimg);
            break;
        default:
            throw std::invalid_argument("Unsupported pixeltype.");
    }

    if (r != 0)
    {
        std::stringstream ss;
        ss << "Error allocating an MImage with width=" << size.w << ", height=" << size.h << ".";
        throw std::runtime_error(ss.str());
    }

    return mimg;
}

/*static*/void MImageHelper::SwapRgb(libCZI::IBitmapData* bd)
{
    switch (bd->GetPixelType())
    {
        case PixelType::Bgr24:
        {
            ScopedBitmapLocker<IBitmapData*> lckBm{ bd };
            MImageHelper::ConvertInplace_RGB24_to_BGR24(bd->GetWidth(), bd->GetHeight(), lckBm.stride, lckBm.ptrDataRoi);
        }

        break;
        case PixelType::Bgr48:
        {
            ScopedBitmapLocker<IBitmapData*> lckBm{ bd };
            MImageHelper::ConvertInplace_RGB48_to_BGR48(bd->GetWidth(), bd->GetHeight(), lckBm.stride, lckBm.ptrDataRoi);
        }

        break;
        default:
            break;
    }
}

/*static*/bool MImageHelper::TryGetPixelType(WolframImageLibrary_Functions imgLibFunctions, MImage mimg, libCZI::PixelType* pixelType)
{
    const auto dt = imgLibFunctions->MImage_getDataType(mimg);
    const auto cs = imgLibFunctions->MImage_getColorSpace(mimg);
    switch (dt)
    {
        case MImage_Type_Bit8:
        {
            switch (cs)
            {
                case MImage_CS_Gray:
                    if (imgLibFunctions->MImage_getChannels(mimg) == 1)
                    {
                        if (pixelType != nullptr) { *pixelType = libCZI::PixelType::Gray8; }
                        return true;
                    }

                    break;
                case MImage_CS_RGB:
                    if (imgLibFunctions->MImage_interleavedQ(mimg) && imgLibFunctions->MImage_getChannels(mimg) == 3)
                    {
                        if (pixelType != nullptr) { *pixelType = libCZI::PixelType::Bgr24; }
                        return true;
                    }

                    break;
                default:
                    break;

            }
        }

        break;
        case MImage_Type_Bit16:
        {
            switch (cs)
            {
                case MImage_CS_Gray:
                    if (imgLibFunctions->MImage_getChannels(mimg) == 1)
                    {
                        if (pixelType != nullptr) { *pixelType = libCZI::PixelType::Gray16; }
                        return true;
                    }

                    break;
                case MImage_CS_RGB:
                    if (imgLibFunctions->MImage_interleavedQ(mimg) && imgLibFunctions->MImage_getChannels(mimg) == 3)
                    {
                        if (pixelType != nullptr) { *pixelType = libCZI::PixelType::Bgr48; }
                        return true;
                    }

                    break;
                default:
                    break;
            }
        }

        break;
        case MImage_Type_Real32:
        {
            if (imgLibFunctions->MImage_getChannels(mimg) == 1)
            {
                if (pixelType != nullptr) { *pixelType = libCZI::PixelType::Gray32Float; }
                return true;
            }

            break;
        }
        default:
            break;
    }

    return false;
}

/*static*/void MImageHelper::ConvertInplace_RGB24_to_BGR24(std::uint32_t w, std::uint32_t h, uint32_t stride, void* ptr)
{
    for (decltype(h) y = 0; y < h; ++y)
    {
        uint8_t* ptrLine = static_cast<uint8_t*>(ptr) + y * static_cast<size_t>(stride);
        for (decltype(w) x = 0; x < w; ++x)
        {
            const uint8_t t = ptrLine[0];
            ptrLine[0] = ptrLine[2];
            ptrLine[2] = t;
            ptrLine += 3;
        }
    }
}

/*static*/void MImageHelper::ConvertInplace_RGB48_to_BGR48(std::uint32_t w, std::uint32_t h, uint32_t stride, void* ptr)
{
    for (decltype(h) y = 0; y < h; ++y)
    {
        uint16_t* ptrLine = static_cast<uint16_t*>(ptr) + y * static_cast<size_t>(stride);
        for (decltype(w) x = 0; x < w; ++x)
        {
            const uint16_t t = ptrLine[0];
            ptrLine[0] = ptrLine[2];
            ptrLine[2] = t;
            ptrLine += 3;
        }
    }
}