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

#include <iostream>

#include "poppler-document.h"
#include "poppler-page.h"
using namespace std;


/*---[ Implement ]----------------------------------------------------------------------------------*/

int main(int argc, const char *argv[]) {

	poppler::document *doc = poppler::document::load_from_file("./sample.pdf");

	const int pagesNbr = doc->pages();
	cout << "page count: " << pagesNbr << endl;

	for (int i = 0; i < pagesNbr; ++i) {
		cout << doc->create_page(i)->text().to_latin1().c_str() << endl;
	}

	return 0;

}


