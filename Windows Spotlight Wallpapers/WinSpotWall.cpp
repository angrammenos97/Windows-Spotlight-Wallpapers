#include <iostream>
#include <fstream>
#include <string>
#pragma warning(disable:4996)

using namespace std;

bool GetImageDimensions(const char *fn, int *x, int *y);

int main()
{
	string userDestination = getenv("userprofile");
	string filesDestination = userDestination + "\\Pictures\\Windows_Wallpapers\\files.txt";

	// First time run
	bool firstTime = false;
	fstream *testFile = new fstream;
	testFile->open(filesDestination);
	if (!testFile->is_open()) {
		system("mkdir \"%USERPROFILE%\\Pictures\\Windows_Wallpapers\"");
		system("mkdir \"%USERPROFILE%\\Pictures\\Windows_Wallpapers\\Older\"");
		firstTime = true;
	}
	else {		
		testFile->close();
		delete testFile;
		//system("del \"%USERPROFILE%\\Pictures\\Windows_Wallpapers\\files.txt\"");
	}

	// Main process
	system("CopyFiles.cmd");
	system("FilesInTxt.cmd");

	fstream myFile;
	myFile.open(filesDestination);
	if (!myFile.is_open()) {
		cout << "Cannot open file \"files.txt\" ." << endl;
		system("pause");
		exit(-1);
	}

	string line;
	int width, height;
	while (getline(myFile, line))
	{
		//cout << line << endl;		
		if (!GetImageDimensions(line.c_str(), &width, &height) || width != 1920 || height != 1080) {
			//cout << "deleting.." << endl;
			string command = "del " + line;
			system(command.c_str());
		}
		//cout << "( " << width << " , " << height << " )" << endl;
	}

	// First time run
	if (firstTime)
		// Open choose background settings
		system("start ms-settings:personalization-background");
	
	//system("pause");
	myFile.close();
	return 0;
}

bool GetImageDimensions(const char *fn, int *x, int *y)
{
	FILE *f = fopen(fn, "rb");
	if (f == 0) return false;
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (len<24)
	{
		fclose(f);
		return false;
	}
	//    cout << fn << endl;
	// Strategy:
	// reading GIF dimensions requires the first 10 bytes of the file
	// reading PNG dimensions requires the first 24 bytes of the file
	// reading JPEG dimensions requires scanning through jpeg chunks
	// In all formats, the file is at least 24 bytes big, so we'll read that always
	unsigned char buf[24];
	fread(buf, 1, 24, f);

	// For JPEGs, we need to read the first 12 bytes of each chunk.
	// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F')
	{
		long pos = 2;
		while (buf[2] == 0xFF)
		{
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB) break;
			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12>len) break;
			fseek(f, pos, SEEK_SET);
			fread(buf + 2, 1, 12, f);
		}
	}

	fclose(f);

	// JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
	{
		*y = (buf[7] << 8) + buf[8];
		*x = (buf[9] << 8) + buf[10];
		//cout << *x << endl;
		return true;
	}

	// GIF: first three bytes say "GIF", next three give version number. Then dimensions
	if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
	{
		*x = buf[6] + (buf[7] << 8);
		*y = buf[8] + (buf[9] << 8);
		return true;
	}

	// PNG: the first frame is by definition an IHDR frame, which gives dimensions
	if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A
		&& buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		*x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		*y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
		return true;
	}

	return false;
}