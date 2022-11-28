#include "file_reader.h"

std::ifstream* openFile(const std::string& URL) {
	std::string path = URL;
	if (path == "/") {
		path = ROOT_PATH;
		path += "index.html";
	}
	else {
		for (int i = 0; path[i] != '\0'; ++i)
		{
			if (path[i] == '/')
				path.replace(i, 1, "\\\\");
		}
		path.insert(0, ROOT_PATH);
	}
	auto file = new std::ifstream(path);
	if (file->is_open()) {
		return file;
	}
	else {
		delete file;
		throw NOT_FOUND;
	}
}

std::string readFile(std::ifstream* file, const std::string data_format) {
	std::string content;
	if (data_format == "jpg") {
		while (!file->eof()) {
			char sym = (char)file->get();
			content += sym;
		}
		file->close();
		using namespace boost::archive::iterators;
		std::stringstream base64_image;
		typedef base64_from_binary<transform_width<const char*, 6, 8>> image_to_base64;
		std::copy(image_to_base64(content.c_str()), image_to_base64(content.c_str() + content.size()), std::ostream_iterator<char>(base64_image));
		content = base64_image.str();
	}
	else {
		while (!file->eof()) {
			std::string str;
			getline(*file, str);
			content += str + '\n';
		}
		file->close();
	}
	return content;
}