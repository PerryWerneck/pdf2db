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
#include <dirent.h>


using namespace std;
using namespace APPNAME;

/*---[ Implement ]----------------------------------------------------------------------------------*/

/// @brief Diretório de onde serão lidas as definições de importação.
static const char *defdir = "./import.d";

/// @brief Verifica se o arquivo termina em .xml
static int xmlFilter(const struct dirent *entry) {
	return APPNAME::string::hasSuffix(entry->d_name,".xml") ? 1 : 0;
}

int main(int argc, const char *argv[]) {

	// Lista de parsers definida
	std::vector<PDFImporter::Parser> parsers;

	// Carrega definições de procedimentos
	struct dirent **namelist;
	int qtdFiles = scandir(defdir, &namelist, xmlFilter, alphasort);
	if(qtdFiles < 0) {
		throw errno;
	}

	for(int file = 0; file < qtdFiles; file++) {

		APPNAME::string filename;
		filename.concat(defdir,"/",namelist[file]->d_name,nullptr);
		free(namelist[file]);

		clog << "Lendo definições de " << filename << "..." << endl;

		pugi::xml_document doc;
		doc.load_file(filename.c_str());

		for(XMLNode top : doc) {

			for(XMLNode node = top.child("procedure"); node; node = node.next_sibling("procedure")) {
				parsers.emplace_back(node);
			}

		}


	}

	free(namelist);

	cout << "Carreguei " << parsers.size() << " formatos de documento." << endl;

	// Carrega arquivos PDF.
	PDFImporter::Document document("./sample.pdf");

#ifdef DEBUG
	size_t ln = 1;
	document.forEach([&ln](const char *line) {
		cout << (ln++) << ":\t" << line << endl;
		return true;
	});
#endif // DEBUG

	for(auto parser = parsers.begin(); parser != parsers.end(); parser++) {

		if(parser->set(document)) {
			cout << "Encontrei documento válido" << endl;
			break;
		}

	}


	return 0;

}


