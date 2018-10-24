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

	namespace PDFImporter {

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

	}


#endif // PDFIMPORPER_H_INCLUDED
