#define  _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<map>
#include"tinyxml2.h"

#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace tinyxml2;
void test1()//change all the "width"
{
	XMLDocument doc;
	doc.LoadFile("E:/001Company/face/dlib-19.2/examples/faces/training.xml");
	XMLElement * dataset = doc.RootElement();
	XMLElement * images = dataset->FirstChildElement("images");
	XMLElement * image = images->FirstChildElement();
	while (image) {
		XMLElement *box = image->FirstChildElement();
		//XMLElement *surNextChild;
		while (box) {
			box->SetAttribute("width", (int)(atof(box->Attribute("width")) * 0.6));
			box = box->NextSiblingElement();
		}
		image = image->NextSiblingElement();
	}
	doc.SaveFile("E:/001Company/face/dlib-19.2/examples/faces/train02.xml");
}
void test2() {//delete all "part"
	XMLDocument doc;
	doc.LoadFile("E:/dlibTrain/rightFaces_H_testset_no_missed_faces.xml");
	XMLElement * dataset = doc.RootElement();
	XMLElement * images = dataset->FirstChildElement("images");
	XMLElement * image = images->FirstChildElement();
	while (image) {
		XMLElement *box = image->FirstChildElement();
		//XMLElement *surNextChild;
		/*while (box) {
			box->SetAttribute("width", (int)(atof(box->Attribute("width")) * 0.6));
			box = box->NextSiblingElement();
		}*/
		while (box) {
			box->DeleteChildren();
			box = box->NextSiblingElement();
		}
		image = image->NextSiblingElement();
	}
	doc.SaveFile("E:/dlibTrain/rightFaces_H_testset_no_missed_faces_no_part.xml");
}


//摘自《代码之美》
//match2(regexp,text)
// 字符     含义
// .        匹配任意的单个字符
// ^        匹配输入字符串的开头
// $        匹配输入字符串的结尾
// *        匹配前一个字符的零个或者多个出现
int matchhere(char *regexp, char *text);
int matchstar(int c, char *regexp, char *text) {// matchstar: search for c*regexp at beginning of text
	do {// a * matches zero or more instances
		if (matchhere(regexp, text)) return 1;
	} while (*text != '\0' && (*text++ == c || c == '.'));
	return 0;
}
int matchhere(char *regexp, char *text) {// matchhere: search for regexp at beginning of text
	if (regexp[0] == '\0') return 1;
	if (regexp[1] == '*') return matchstar(regexp[0], regexp + 2, text);
	if (regexp[0] == '$' && regexp[1] == '\0') return *text == '\0';
	if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text)) return matchhere(regexp + 1, text + 1);
	return 0;
}
int match2(char *regexp, char *text) {// match: search for regexp anywhere in text
	if (regexp[0] == '^') return matchhere(regexp + 1, text);
	do {// must look even if string is empty
		if (matchhere(regexp, text)) return 1;
	} while (*text++ != '\0');
	return 0;
}

//match1(regexp,text)
// 字符     含义
// ?        匹配任意的单个字符
// *        匹配零个或者多个字符
int match_imp(const char *d, int dcur, const char *s, int scur) {
	if (!d[dcur]) return (!s[scur]) ? 1 : 0;
	if (d[dcur] == '?') return match_imp(d, dcur + 1, s, scur + 1);
	else if (d[dcur] == '*') {
		do {
			if (match_imp(d, dcur + 1, s, scur)) return 1;
		} while (s[scur++]);
		return 0;
	}
	else return (tolower(d[dcur]) == tolower(s[scur]) && match_imp(d, dcur + 1, s, scur + 1)) ? 1 : 0;
}
int match1(char* s1, char* s2) {
	return match_imp(s1, 0, s2, 0);
}
void test3() {
	printf("%d==match1(abc ,abc)\n", match1("abc", "abc"));
	printf("%d==match1(a?c ,abc)\n", match1("a?c", "abc"));
	printf("%d==match1(a*c ,abc)\n", match1("a*c", "abc"));
	printf("-------------------\n");
	printf("%d==match1(abc ,abd)\n", match1("abc", "abd"));
	printf("%d==match1(a?c ,abd)\n", match1("a?c", "abd"));
	printf("%d==match1(a*c ,abd)\n", match1("a*c", "abd"));
	printf("\n");
	printf("%d==match2(abc ,abc)\n", match2("abc", "abc"));
	printf("%d==match2(^a  ,abc)\n", match2("^a", "abc"));
	printf("%d==match2(c$  ,abc)\n", match2("c$", "abc"));
	printf("%d==match2(a.c ,abc)\n", match2("a.c", "abc"));
	printf("%d==match2(a.*c,abc)\n", match2("a.*c", "abc"));
	printf("-------------------\n");
	printf("%d==match2(ABC ,abc)\n", match2("ABC", "abc"));
	printf("%d==match2(^B  ,abc)\n", match2("^B", "abc"));
	printf("%d==match2(A$  ,abc)\n", match2("A$", "abc"));
	printf("%d==match2(a..c,abc)\n", match2("a..c", "abc"));
	printf("%d==match2(a.*d,abc)\n", match2("a.*d", "abc"));
	printf("-------------------\n");
	printf("%d==match1(111076519_*.jpg ,111076519_1234.jpg)\n", match1("111076519_*.jpg", "111076519_1234.jpg"));
	printf("%d==match1(111076519_*_mirror.jpg ,111076519_2234_mirror.jpg)\n", match1("111076519_*_mirror.jpg", "111076519_2234_mirror.jpg"));
	printf("%d==match1(111076519_*.jpg ,111076519_1234_mirror.jpg)\n", match1("111076519_*.jpg", "111076519_1234_mirror.jpg"));
	printf("-------------------\n");
	printf("%d==match1(111076519_*_mirror.jpg ,111076519_2234.jpg)\n", match1("111076519_*_mirror.jpg", "111076519_2234.jpg"));

}
int findChar(const char * str,char c) {
	int index = -1;
	if (str != NULL) {
		while (str[index + 1]) {
			if (c == str[++index])break;
		}
	}
	return index;
}
string replaceChar(const char * str, char c) {
	string newStr;
	if (str != NULL) {
		int index = 0,flag=0;
		while (str[index]) {
			if (flag != 1)
				newStr += str[index];
			if (c == str[index]) {
				flag++;
				if (flag ==2) {
					newStr += "*_";
				}
			}
			if (str[index] == '.' && flag == 1) {
				newStr += "*";
				newStr += str[index];
				flag++;
			}
			index++;
		}
	}
	return newStr;
}
void test4() {//只对文件中afw的合并box
	map<string,XMLElement*> xmlEles;
	XMLDocument doc;
	doc.LoadFile("E:/dlibTrain/labels_ibug_300W.xml");
	XMLElement * dataset = doc.RootElement();
	XMLElement * images = dataset->FirstChildElement("images");
	XMLElement * image = images->FirstChildElement();
	string awfFile = "afw";
	while (image) {
		string fileName = image->Attribute("file");
		XMLElement* nextImage = image->NextSiblingElement();
		if (fileName.find(awfFile) !=string::npos) {
			pair<map<string, XMLElement*>::iterator, bool>insertPair;
			insertPair = xmlEles.insert(pair<string, XMLElement*>(replaceChar(image->Attribute("file"), '_'), image));
			
			if (insertPair.second == false) {
				XMLElement * box = image->FirstChildElement();
				while (box) {
					insertPair.first->second->InsertEndChild(box);
					box = box->NextSiblingElement();
				}
				images->DeleteChild(image);
			}
		}
		image = nextImage;
	}
	doc.SaveFile("E:/dlibTrain/300W_mergeAll.xml");
}

void test5() {//split the file with "awf","ibug","helen","lfpw" into four smaller xml file
	XMLDocument doc, awfDoc, helenDoc, lfpwDoc, ibugDoc;
	string awfFile = "afw", helenFile = "helen/trainset/", lfpwFile = "lfpw", ibugFile = "ibug";

	/*XMLElement* root = awfDoc.NewElement("dataset");
	awfDoc.InsertFirstChild(root);
	root = awfDoc.NewElement("images");
	awfDoc.RootElement()->InsertFirstChild(root);
*/
	XMLElement* root = helenDoc.NewElement("dataset");
	helenDoc.InsertFirstChild(root);
	root = helenDoc.NewElement("images");
	helenDoc.RootElement()->InsertFirstChild(root);

	root = lfpwDoc.NewElement("dataset");
	lfpwDoc.InsertFirstChild(root);
	root = lfpwDoc.NewElement("images");
	lfpwDoc.RootElement()->InsertFirstChild(root);

	root = ibugDoc.NewElement("dataset");
	ibugDoc.InsertFirstChild(root);
	root = ibugDoc.NewElement("images");
	ibugDoc.RootElement()->InsertFirstChild(root);

	doc.LoadFile("E:/dlibTrain/300Wall.xml");
	XMLElement * dataset = doc.RootElement();
	XMLElement * images = dataset->FirstChildElement("images");
	XMLElement * image = images->FirstChildElement();
	while (image) {
		string fileName = image->Attribute("file");
		//if (fileName.find(awfFile) !=string::npos) {
			//awfDoc.RootElement()->FirstChildElement()->InsertEndChild(image->DeepClone(&awfDoc));
		//}
		//else 
		if (fileName.find(helenFile) != string::npos) {
			helenDoc.RootElement()->FirstChildElement()->InsertEndChild(image->DeepClone(&helenDoc));
		}
		else if (fileName.find(lfpwFile) != string::npos) {
			lfpwDoc.RootElement()->FirstChildElement()->InsertEndChild(image->DeepClone(&lfpwDoc));
		}
		else if (fileName.find(ibugFile) != string::npos) {
			ibugDoc.RootElement()->FirstChildElement()->InsertEndChild(image->DeepClone(&ibugDoc));
		}
		image = image->NextSiblingElement();
	}
	//awfDoc.SaveFile("E:/dlibTrain/300allAFW.xml");
	helenDoc.SaveFile("E:/dlibTrain/300trainHELEN.xml");
	lfpwDoc.SaveFile("E:/dlibTrain/300allLFPW.xml");
	ibugDoc.SaveFile("E:/dlibTrain/300allIBUG.xml");
}
void test6() {//get all images xml that contain 'part' from 300allA_H_L_I_Merge.xml and labels_ibug_300W.xml
	XMLDocument A_H_L_I_MergeDoc, allDoc, newDoc;
	
	XMLElement* root = newDoc.NewElement("dataset");
	newDoc.InsertFirstChild(root);
	root = newDoc.NewElement("images");
	newDoc.RootElement()->InsertFirstChild(root);

	A_H_L_I_MergeDoc.LoadFile("E:/dlibTrain/300allA_H_L_I_Merge.xml");
	allDoc.LoadFile("E:/dlibTrain/300W_mergeAll.xml");
	XMLElement * dataset_AHLI = A_H_L_I_MergeDoc.RootElement();
	XMLElement * images_AHLI = dataset_AHLI->FirstChildElement("images");
	XMLElement * image_AHLI = images_AHLI->FirstChildElement();

	XMLElement * dataset_All = allDoc.RootElement();
	XMLElement * images_All = dataset_All->FirstChildElement("images");
	XMLElement * image_All = images_All->FirstChildElement();

	map<string, XMLElement*> xmlEles;
	while (image_AHLI) {
		pair<map<string, XMLElement*>::iterator, bool>insertPair;
		insertPair = xmlEles.insert(pair<string, XMLElement*>(image_AHLI->Attribute("file"), image_AHLI));
		image_AHLI = image_AHLI->NextSiblingElement();
	}

	while (image_All) {
		map<string, XMLElement*>::iterator it;
		it = xmlEles.find(image_All->Attribute("file"));
		
		if (it != xmlEles.end()) {//found
			//XMLElement * image_AHLI_ = it->second;
			newDoc.RootElement()->FirstChildElement()->InsertEndChild(image_All->DeepClone(&newDoc));
		}
		image_All = image_All->NextSiblingElement();
	}

	newDoc.SaveFile("E:/dlibTrain/300all_has_Part_A_H_L_I_Merge.xml");
}
void darkenHalfImageFromXML() {
	XMLDocument doc;
	doc.LoadFile("E:/dlibTrain/300allIBUG.xml");
	XMLElement * dataset = doc.RootElement();
	XMLElement * images = dataset->FirstChildElement("images");
	XMLElement * image = images->FirstChildElement();
	while (image) {
		string imageFile = image->Attribute("file");
		XMLElement * box = image->FirstChildElement();
		if (box) {
			cv::Mat src = cv::imread(imageFile);
			int height = atoi(box->Attribute("height")),width= atoi(box->Attribute("width"))*0.4,
				left= (left=atoi(box->Attribute("left")))<0?0:left,top= (top=atoi(box->Attribute("top")))<0?0:top;

			cv::Mat dark(height,width, CV_8UC3, cv::Scalar(0));
			dark.copyTo(src(cv::Rect(left, top, dark.cols, dark.rows)));
			cv::imwrite(imageFile, src);
		}
		image = image->NextSiblingElement();
	}
}
int main() {
	test2();
	//test4();
	//test5();
	//darkenHalfImageFromXML();
	//test6();
}