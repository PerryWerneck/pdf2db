/**
 * @file pdfimporter/src/main/document.cc
 *
 * @brief
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

 #include <pdfimporter.h>
 #include <poppler-document.h>
 #include "poppler-page.h"

/*---[ Implement ]----------------------------------------------------------------------------------*/

 static PwPET::string toString(poppler::ustring str){

	if(!str.length()) {
		return PwPET::string();
	}

    auto buf = str.to_utf8();

	return PwPET::string(buf.data(), buf.size());

 }

 PDFImporter::Document::Document(const char *name) {

	poppler::document *doc = poppler::document::load_from_file(name);

	for (int pg = 0; pg < doc->pages(); ++pg) {

		pages.emplace_back();

		toString(doc->create_page(pg)->text()).split(pages.back(),'\n');

		for(auto ln = pages.back().begin(); ln != pages.back().end(); ln++) {
			ln->strip();
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



