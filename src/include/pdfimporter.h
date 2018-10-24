/**
 * @file pdfimporter/src/include/pdfimporter.h
 *
 * @brief Definições para o importador de PDF C++
 *
 * @author perry.werneck@gmail.com
 *
 * $URL$
 * $Revision$ $Author$
 *
 */

#ifndef PDFIMPORPER_H_INCLUDED

	#define PDFIMPORPER_H_INCLUDED 1

	#include <config.h>
	#include <components/core/defs.h>
	#include <components/core/string.h>
	#include <functional>
	#include <pugixml.hpp>

	namespace PDFImporter {

		/// @brief Documento PDF já convertido para texto sem espaços.
		class Document {
		private:

			class Page : public std::vector<PwPET::string> {
			private:
				friend class PDFImporter::Document;

			public:
				Page() = default;

			};

			std::vector<Page> pages;

		public:
			Document(const char *name);

			bool forEach(std::function<bool(const char *line)> callback) const;



		};

		/// @brief Filtro a aplicar num documento.
		class Filter {
		public:
			Filter() = default;
			virtual ~Filter();

			/// @brief Verifica se o documento atende o filtro.
			virtual bool test(const Document &document) = 0;

		};

		/// @brief Parser de documento.
		class Parser {
		private:

			/// @brief Lista de filtros a aplicar no documento.
			std::vector<Filter *> filters;

		public:

			Parser(const pugi::xml_node &node);
			~Parser();

			/// @brief Faz o parse do documento.
			bool set(const Document &document);

		};

	}


#endif // PDFIMPORPER_H_INCLUDED
