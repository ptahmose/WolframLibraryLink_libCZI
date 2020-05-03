#include "WolframLibLinkUtils.h"
#include <cstdint>
#include <cmath>
#include <limits>
#include <stdexcept>

using namespace std;

/*static*/bool WolframLibLinkUtils::TryGetAsInt32(int* out, size_t outCount, MNumericArray numArray, WolframNumericArrayLibrary_Functions naFuns)
{
    // check whether the count is as expect
    if (naFuns->MNumericArray_getRank(numArray) != 1)
    {
        return false;
    }

    const mint* dimensions = naFuns->MNumericArray_getDimensions(numArray);
    if (dimensions[0] <= 0 || static_cast<size_t>(dimensions[0]) != outCount)
    {
        return false;
    }

    if (static_cast<size_t>(naFuns->MNumericArray_getFlattenedLength(numArray)) != outCount)
    {
        return false;
    }

    auto type = naFuns->MNumericArray_getType(numArray);

    switch (type)
    {
    case MNumericArray_Type_Bit8:
    {
        const std::int8_t* p = (const std::int8_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_UBit8:
    {
        const std::uint8_t* p = (const std::uint8_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_Bit16:
    {
        const std::int16_t* p = (const std::int16_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_UBit16:
    {
        const std::uint16_t* p = (const std::uint16_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_Bit32:
    {
        const std::int32_t* p = (const std::int32_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_UBit32:
    {
        const std::uint32_t* p = (const std::uint32_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            auto v = p[i];
            if (v > static_cast<uint32_t>(std::numeric_limits<int32_t>::max()))
            {
                return false;
            }

            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_Bit64:
    {
        const std::int64_t* p = (const std::int64_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            auto v = p[i];
            if (v > std::numeric_limits<int32_t>::max() || v < std::numeric_limits<int32_t>::min())
            {
                return false;
            }

            out[i] = static_cast<int>(p[i]);
        }

        break;
    }
    case MNumericArray_Type_UBit64:
    {
        const std::uint64_t* p = (const std::uint64_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            auto v = p[i];
            if (v > std::numeric_limits<int32_t>::max())
            {
                return false;
            }

            out[i] = (int)p[i];
        }

        break;
    }
    case MNumericArray_Type_Real32:
    {
        const float* p = (const float*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            auto v = nearbyint(p[i]);
            if (v > std::numeric_limits<int32_t>::max() || v < std::numeric_limits<int32_t>::min())
            {
                return false;
            }

            out[i] = (int)p[i];
        }

        break;
    }
    case MNumericArray_Type_Real64:
    {
        const double* p = (const double*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            auto v = nearbyint(p[i]);
            if (v > std::numeric_limits<int32_t>::max() || v < std::numeric_limits<int32_t>::min())
            {
                return false;
            }

            out[i] = (int)p[i];
        }

        break;
    }
    default:
        return false;
    }

    return true;
}

/*static*/bool WolframLibLinkUtils::TryGetAsFloat32(float* out, size_t outCount, MNumericArray numArray, WolframNumericArrayLibrary_Functions naFuns)
{
    // check whether the count is as expect
    if (naFuns->MNumericArray_getRank(numArray) != 1)
    {
        return false;
    }

    const mint* dimensions = naFuns->MNumericArray_getDimensions(numArray);
    if (dimensions[0] <= 0 || static_cast<size_t>(dimensions[0]) != outCount)
    {
        return false;
    }

    if (static_cast<size_t>(naFuns->MNumericArray_getFlattenedLength(numArray)) != outCount)
    {
        return false;
    }

    auto type = naFuns->MNumericArray_getType(numArray);

    switch (type)
    {
    case MNumericArray_Type_Bit8:
    {
        const std::int8_t* p = (const std::int8_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_UBit8:
    {
        const std::uint8_t* p = (const std::uint8_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_Bit16:
    {
        const std::int16_t* p = (const std::int16_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_UBit16:
    {
        const std::uint16_t* p = (const std::uint16_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_Bit32:
    {
        const std::int32_t* p = (const std::int32_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = static_cast<float>(p[i]);
        }

        break;
    }
    case MNumericArray_Type_UBit32:
    {
        const std::uint32_t* p = (const std::uint32_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = static_cast<float>(p[i]);
        }

        break;
    }
    case MNumericArray_Type_Bit64:
    {
        const std::int64_t* p = (const std::int64_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = static_cast<float>(p[i]);
        }

        break;
    }
    case MNumericArray_Type_UBit64:
    {
        const std::uint64_t* p = (const std::uint64_t*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = static_cast<float>(p[i]);
        }

        break;
    }
    case MNumericArray_Type_Real32:
    {
        const float* p = (const float*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = p[i];
        }

        break;
    }
    case MNumericArray_Type_Real64:
    {
        const double* p = (const double*)naFuns->MNumericArray_getData(numArray);
        for (size_t i = 0; i < outCount; ++i)
        {
            out[i] = static_cast<float>(p[i]);
        }

        break;
    }
    default:
        return false;
    }

    return true;
}

/*static*/void WolframLibLinkUtils::ThrowIfValueIsNotInt32OrNegative(mint i)
{
    if (i<0 || i>numeric_limits<int>::max())
    {
        throw invalid_argument("Argument is not a non-negative int32");
    }
}