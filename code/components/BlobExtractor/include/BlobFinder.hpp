
#ifndef BLOB_FINDER_HPP
#define BLOB_FINDER_HPP

#include "Blob.hpp"

class BlobFinder
{
public:
    void FindBlob(cv::Mat image, std::vector<Blob> &blobs);

private:
    const uint8_t BACKGROUND = 0;
    const uint8_t DEPTH_THRESHOLD = 150; // 150 - 255 are white colors - far away 0-150 black colors - closer
};

#endif // BLOB_FINDER_HPP