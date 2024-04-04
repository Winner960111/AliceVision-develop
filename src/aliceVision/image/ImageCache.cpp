// This file is part of the AliceVision project.
// Copyright (c) 2022 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "ImageCache.hpp"

#include <aliceVision/system/Logger.hpp>

namespace aliceVision {
namespace image {

CacheValue::CacheValue() {}

CacheValue CacheValue::wrap(std::shared_ptr<Image<unsigned char>> img)
{
    CacheValue value;
    value.imgUChar = img;
    return value;
}

CacheValue CacheValue::wrap(std::shared_ptr<Image<float>> img)
{
    CacheValue value;
    value.imgFloat = img;
    return value;
}

CacheValue CacheValue::wrap(std::shared_ptr<Image<RGBColor>> img)
{
    CacheValue value;
    value.imgRGB = img;
    return value;
}

CacheValue CacheValue::wrap(std::shared_ptr<Image<RGBfColor>> img)
{
    CacheValue value;
    value.imgRGBf = img;
    return value;
}

CacheValue CacheValue::wrap(std::shared_ptr<Image<RGBAColor>> img)
{
    CacheValue value;
    value.imgRGBA = img;
    return value;
}

CacheValue CacheValue::wrap(std::shared_ptr<Image<RGBAfColor>> img)
{
    CacheValue value;
    value.imgRGBAf = img;
    return value;
}

int CacheValue::useCount() const
{
    if (imgUChar)
    {
        return imgUChar.use_count();
    }
    if (imgFloat)
    {
        return imgFloat.use_count();
    }
    if (imgRGB)
    {
        return imgRGB.use_count();
    }
    if (imgRGBf)
    {
        return imgRGBf.use_count();
    }
    if (imgRGBA)
    {
        return imgRGBA.use_count();
    }
    if (imgRGBAf)
    {
        return imgRGBAf.use_count();
    }
    return 0;
}

unsigned long long int CacheValue::memorySize() const
{
    if (imgUChar)
    {
        return imgUChar->memorySize();
    }
    if (imgFloat)
    {
        return imgFloat->memorySize();
    }
    if (imgRGB)
    {
        return imgRGB->memorySize();
    }
    if (imgRGBf)
    {
        return imgRGBf->memorySize();
    }
    if (imgRGBA)
    {
        return imgRGBA->memorySize();
    }
    if (imgRGBAf)
    {
        return imgRGBAf->memorySize();
    }
    return 0;
}

ImageCache::ImageCache(float capacity_MiB, float maxSize_MiB, const ImageReadOptions& options)
  : _info(capacity_MiB, maxSize_MiB),
    _options(options)
{}

ImageCache::~ImageCache() {}

std::string ImageCache::toString() const
{
    std::string description = "Image cache content (LRU to MRU): ";

    for (const CacheKey& key : _keys)
    {
        std::string keyDesc = key.filename + ", nbChannels: " + std::to_string(key.nbChannels) + ", typeDesc: " + std::to_string(key.typeDesc) +
                              ", downscaleLevel: " + std::to_string(key.downscaleLevel) +
                              ", usages: " + std::to_string(_imagePtrs.at(key).useCount()) +
                              ", size: " + std::to_string(_imagePtrs.at(key).memorySize());
        description += "\n * " + keyDesc;
    }

    std::string memUsageDesc = "\nMemory usage: "
                               "\n * capacity: " +
                               std::to_string(_info.capacity) + "\n * max size: " + std::to_string(_info.maxSize) +
                               "\n * nb images: " + std::to_string(_info.nbImages) + "\n * content size: " + std::to_string(_info.contentSize);
    description += memUsageDesc;

    std::string statsDesc = "\nUsage statistics: "
                            "\n * nb load from disk: " +
                            std::to_string(_info.nbLoadFromDisk) + "\n * nb load from cache: " + std::to_string(_info.nbLoadFromCache) +
                            "\n * nb remove unused: " + std::to_string(_info.nbRemoveUnused);
    description += statsDesc;

    return description;
}

}  // namespace image
}  // namespace aliceVision
