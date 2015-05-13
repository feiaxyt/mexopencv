/**
 * @file matchTemplate.cpp
 * @brief mex interface for matchTemplate
 * @author Kota Yamaguchi
 * @date 2011
 */
#include "mexopencv.hpp"
using namespace std;
using namespace cv;

namespace {
/** Distance types for Distance Transform and M-estimators
 */
const ConstMap<std::string,int> MatchMethod = ConstMap<std::string,int>
    ("SqDiff",       cv::TM_SQDIFF)
    ("SqDiffNormed", cv::TM_SQDIFF_NORMED)
    ("CCorr",        cv::TM_CCORR)
    ("CCorrNormed",  cv::TM_CCORR_NORMED)
    ("CCoeff",       cv::TM_CCOEFF)
    ("CCoeffNormed", cv::TM_CCOEFF_NORMED);
}

/**
 * Main entry called from Matlab
 * @param nlhs number of left-hand-side arguments
 * @param plhs pointers to mxArrays in the left-hand-side
 * @param nrhs number of right-hand-side arguments
 * @param prhs pointers to mxArrays in the right-hand-side
 */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
    // Check the number of arguments
    if (nrhs<2 || ((nrhs%2)!=0) || nlhs>1)
        mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    
    // Argument vector
    vector<MxArray> rhs(prhs,prhs+nrhs);
    
    // Option processing
    int method = cv::TM_SQDIFF;
    for (int i=2; i<nrhs; i+=2) {
        string key = rhs[i].toString();
        if (key=="Method") {
            if (rhs[i+1].isChar())
                method = MatchMethod[rhs[i+1].toString()];
            else
                method = rhs[i+1].toInt();
        }
        else
            mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
    }
    
    // Process
    Mat image = (rhs[0].isUint8()) ? rhs[0].toMat(CV_8U) : rhs[0].toMat(CV_32F);
    Mat tmpl = (rhs[1].isUint8()) ? rhs[1].toMat(CV_8U) : rhs[1].toMat(CV_32F);;
    Mat result;
    matchTemplate(image,tmpl,result,method);
    plhs[0] = MxArray(result);
}
