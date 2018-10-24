/**
 * @file src/pdfimporter.cc
 *
 * @brief Ponto de entrada para o aplicativo de importação de exames em PDF
 *
 * @author perry.werneck@gmail.com
 *
 * Referências:
 *
 * <https://stackoverflow.com/questions/2732178/extracting-text-from-pdf-with-poppler-c>
 * <http://marpirk.github.io/poppler-cpp-doc/classpoppler_1_1document.html>
 *
 */

#include <components/core/defs.h>
#include <components/core/string.h>
#include <iostream>
#include <vector>

#include "poppler-document.h"
#include "poppler-page.h"
using namespace std;

/*---[ Implement ]----------------------------------------------------------------------------------*/

static PwPET::string toString(poppler::ustring str){

	if(!str.length()) {
		return PwPET::string();
	}

    auto buf = str.to_utf8();

	return PwPET::string(buf.data(), buf.size());

}

int main(int argc, const char *argv[]) {

	poppler::document *doc = poppler::document::load_from_file("./sample.pdf");

	const int pagesNbr = doc->pages();

	for (int i = 0; i < pagesNbr; ++i) {

		std::vector<PwPET::string> text;
		toString(doc->create_page(i)->text()).split(text,'\n');

		for(auto ln = text.begin(); ln != text.end(); ln++) {
			ln->strip();
		}

		for(size_t l = 0; l < text.size(); l++) {
			cout << "P:" << (i+1) << " L:" << (l+1) << "=" << text[l] << endl;
		}

	}

	return 0;

}


