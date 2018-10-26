/**
 * @file src/main/document.cc
 *
 * @brief
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <cstring>
 #include <pdf2db.h>
 #include <poppler-document.h>
 #include "poppler-page.h"

/*---[ Implement ]----------------------------------------------------------------------------------*/

 static void split(poppler::ustring s, vector<string> &lines){

	if(!s.length()) {
		return;
	}

    auto buf = s.to_utf8();
    auto str = buf.data();
    size_t from = 0;

	while(from < buf.size()) {

		auto ptr = strchr(str+from,'\n');
		if(!ptr) {
			break;
		}

		size_t sz = (ptr - str) - from;
		lines.emplace_back((str+from),sz);
		from += (sz+1);

	}

	if(from < buf.size()) {
		lines.emplace_back(str+from);
	}

//	return string(buf.data(), buf.size());

 }

 PDFImporter::Document::Document(const char *name) {

	poppler::document *doc = poppler::document::load_from_file(name);

	for (int pg = 0; pg < doc->pages(); ++pg) {

		pages.emplace_back();

		split(doc->create_page(pg)->text(),pages.back());

		for(auto ln = pages.back().begin(); ln != pages.back().end(); ln++) {
			strip(*ln);
		}

	}

	delete doc;

 }

 bool PDFImporter::Document::forEach(std::function<bool(const char *line)> callback) const {

	for(auto page : pages) {

		for(auto line : page) {

			if(!callback(line.c_str())) {
				return false;
			}

		}

	}

	return true;

 }

 const string & PDFImporter::Document::getString(unsigned int page, unsigned int line) const {

	if(!(page && line)) {
		throw EINVAL;
	}

	page--;
	line--;

	if(page > pages.size()) {
		throw EINVAL;
	}

	const Page &pg = pages[page];

	if(line > pg.size()) {
		throw EINVAL;
	}

	return pg[line];

 }

 void PDFImporter::Document::get(const unsigned int page, const unsigned int line, std::vector<string> &text) const {

	if(page) {

		pages.at(page-1).get(line,text);

	} else {

		for(auto pg = pages.begin(); pg != pages.end(); pg++) {
            pg->get(line,text);
		}

	}

 }

 void PDFImporter::Document::Page::get(const unsigned int line, std::vector<string> &text) const {

	if(line) {

		text.push_back(this->at(line-1));

	} else {

		for(auto ln = this->begin(); ln != this->end(); ln++) {
			text.push_back(ln->c_str());
		}

	}

 }



