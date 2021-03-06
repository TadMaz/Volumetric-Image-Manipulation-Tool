// volimage.h

#ifndef VOLIMAGE_H
#define VOLIMAGE_H

namespace MZRTAD001
{   
    class VolImage
    {
    private:
        int width, height; // width and height of image stack
        std::vector<unsigned char**> slices; // data for each slice, in order

    public:
        MZRTAD001::VolImage(); // default constructor - define in .cpp
        MZRTAD001::~VolImage(); // destructor - define in .cpp file
        // populate the object with images in stack and
        //set member variables define in .cpp
        
        bool readImages(std::string baseName);
        // compute difference map and write out; define in .cpp
        
        void diffmap(int sliceI, int sliceJ, std::string output_prefix);
        // extract slice sliceId and write to output - define in .cpp
        
        void extract(int sliceId, std::string output_prefix);
        // number of bytes uses to store image data bytes
        //and pointers (ignore vector<> container, dims etc)
        
        int volImageSize(void); // define in .cpp

        int getNumberOfSlices(void);

        void extractRowAcrossSlices(int rowNo);
    };
}
#endif 