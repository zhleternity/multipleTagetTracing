#include <iostream>
#include <io.h>
#include <direct.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video.hpp>





using namespace std;
using namespace cv;


class ImageVideoTrans
{
public:
	//ImageVideoTrans();
	//~ImageVideoTrans();
	//transform frame image sequence to avi video
	int ImageToVideo(char* outDir, char* videoName, char* inputDir, int startFrame, int endFrame, int imgW, int imgH, char* imgExt, double fps, int isColor, int fourcc)
	{
		//�ж������ļ����Ƿ����
		if (_access(inputDir, 0) == -1)
		{
			std::cout << "the input directory does not exist!" << std::endl;
			return 0;
		}
		//�ж�����ļ����Ƿ񴴽� ��û���򴴽�����ΪNULL��Ĭ�ϵ�ǰ����Ŀ¼
		char fullVideoName[255];//�����Ƶ�������ļ�����·��+�ļ���
		strcpy_s(fullVideoName, "");
		if (outDir == NULL)
		{
			sprintf_s(fullVideoName, "%s", videoName);//��videoName��ӡ��һ���ַ���������fullVideoName �� 
		}
		else
		{
			if (_access(outDir, 0) == -1)
			{
				_mkdir(outDir);
			}
			sprintf_s(fullVideoName, "%s%s", outDir, videoName);//���ַ���outDir��videoName������������ӡ��������fullVideoName��
		}
		int frameCount = 0;
		//CvVideoWriter *pWriter = NULL;
		Size size = Size(imgW, imgH);
		VideoWriter pWriter(videoName, fourcc, fps, size, isColor);//CREATE WRITER

		cv::Mat pImg;
		char cur_fn[500];//��ʾĳ��ͼƬ��·��
		while (startFrame <= endFrame)
		{
			strcpy(cur_fn, "");
			char name[] = ".jpg";
			sprintf(cur_fn, "%s%s%05d%s", inputDir, imgExt, startFrame, name);//need to change 
			//cout <<"�����ļ�����Ϊ��"<< cur_fn << endl;
			pImg = imread(cur_fn, isColor);
			if (pImg.empty())
			{
				cout << "can't open an image file" << endl;
				return frameCount;
			}
			pWriter << pImg;
			waitKey(1);
			cout << "Write frame " << startFrame << std::endl;
			startFrame++;
			pImg.release();
			frameCount++;
		}
		//cvReleaseVideoWriter(&pWriter);
		pWriter.release();
		rename(videoName, fullVideoName);//�ƶ��ļ���ָ���ļ���
		return  frameCount;
	}

	//����Ƶת��ΪͼƬ���� ��������Ƶ�ֽ�õ���ͼƬ��֡�� ĿǰOpenCVֻ֧��AVI��ʽ ���ʹ��֮ǰ��Ҫ
	//����Ƶת����AVI��ʽ

	int  VideoToImage(char* videoName, char* outDir, char* imgExt, int maxFrameCount)
	{
		VideoCapture cap(videoName);
		if (!cap.isOpened())
		{
			cout << "Failed open video file!" << endl;
			return 1;
		}
		if (cap.get(CAP_PROP_FRAME_COUNT) < 2)
		{
			cout << "The video must have at least two frames." << endl;
			return 1;
		}
		//����ͼƬ���ļ���·��һ��Ҫ�У���ΪOpenCV�����Զ������ļ���
		if (_access(outDir, 0) == -1)
		{
			recursive_mkdir(outDir);
			std::cout << "the ouput directory does not exist, and the have been created autonomously!" << std::endl;
		}
		char cur_fn[255];//���浱ǰ֡����ͼƬ���ļ���
		cv::Mat pImg;
		int frame = 0;
		double rate = cap.get(CAP_PROP_FPS);
		double delay = 1000 / rate;
		cap.read(pImg);
		while (!pImg.empty() && (frame<maxFrameCount))
		{
			frame++;
			strcpy_s(cur_fn, "");
			sprintf_s(cur_fn, "%s%d%s", outDir, frame, imgExt);//����������ʺ����� 123.jpg 124.jpg��ͼƬ����
			imwrite(cur_fn, pImg);
		}
		pImg.release();
		cap.release();
		return frame;
	}


	//�ú���������������ϣ��Զ������༶Ŀ¼
	int recursive_mkdir(char *dir)
	{
		//�ֽ�·����E:\\AA\\BB\\CC\\
		 //
		string str = dir;
		int index = 0;
		int i = 0;
		while (1)
		{
			string::size_type pos = str.find("\\", index);
			string str1;
			str1 = str.substr(0, pos);
			if (pos != -1 && i > 0)
			{
				if (_access(str1.c_str(), 0) == -1)
				{
					_mkdir(str1.c_str());
				}
			}
			if (pos == -1)
			{
				break;
			}
			i++;
			index = pos + 1;
		}
		return 0;
	}

	/*void iv_trans(int startFrame, int endFrame, char *imgExt, double fps, int flag){
		if (flag)
			ImageToVideo();
	}*/

};



