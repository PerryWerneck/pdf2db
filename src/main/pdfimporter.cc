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

#include <pdfimporter.h>
#include <iostream>
#include <vector>

using namespace std;

/*---[ Implement ]----------------------------------------------------------------------------------*/

int main(int argc, const char *argv[]) {

	PDFImporter::Document file("./sample.pdf");

	file.forEach([](const char *line) {
		cout << line << endl;
		return true;
	});

	return 0;

}


