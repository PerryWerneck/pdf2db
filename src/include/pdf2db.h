/**
 * @file src/include/pdf2db.h
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
	#include <string>
	#include <functional>
	#include <pugixml.hpp>
	#include <cppdb/frontend.h>
	#include <cstdio>
	#include <iostream>

	#ifdef DEBUG
		#define trace( fmt, ... )	fprintf(stderr,"%s(%d) " fmt "\n", __FILE__, __LINE__, __VA_ARGS__ );fflush(stderr)
		#define debug( fmt, ... )	fprintf(stderr,"%s(%d) " fmt "\n", __FILE__, __LINE__, __VA_ARGS__ );fflush(stderr)
	#else
		#define trace( fmt, ... )	/* */
		#define debug( fmt, ... )	/* */
	#endif // DEBUG


	using std::vector;
	using std::string;
	using XMLNode = pugi::xml_node;

	extern char * chomp(char *str) noexcept;
	extern char * chug (char *str) noexcept;
	extern char * strip(char *str) noexcept;
	extern string & strip(string &str) noexcept;
	extern bool hasSuffix(const char *str, const char *suffix) noexcept;

	namespace PDFImporter {

		/// @brief Documento PDF já convertido para texto sem espaços.
		class Document {
		private:

			class Page : public std::vector<string> {
			private:
				friend class PDFImporter::Document;

			public:
				Page() = default;

			};

			std::vector<Page> pages;

		public:
			Document(const char *name);

			bool forEach(std::function<bool(const char *line)> callback) const;

			const string & getString(unsigned int page, unsigned int line) const;

			inline const char * get(unsigned int page, unsigned int line) const {
				return getString(page,line).c_str();
			}


		};

		/// @brief Filtro a aplicar num documento.
		class Filter {
		protected:
			Filter() = default;

		public:

			/// @brief Cria filtro de acordo com a descrição XML.
			static Filter * create(const XMLNode &node);

			virtual ~Filter() {};

			/// @brief Verifica se o documento atende o filtro.
			virtual bool test(const Document &document) = 0;

		};

		/// @brief Valor extraido do documento.
		class Content {
		private:

			/// @brief Nome da propriedade
			string name;

		protected:
			Content(const XMLNode &node);

			/// @brief De que página será extraída.
			unsigned int page;

		public:

			virtual ~Content() { }

			/// @brief Cria conteúdo de acordo com a descrição XML.
			static Content * create(const XMLNode &node);

			/// @brief Extrai conteúdo do documento.
			virtual bool set(const Document &document) = 0;

			/// @brief Reinicia o parser.
			virtual void reset() = 0;

			/// @brief Obtêm o nome do valor
			const string & getName() const {
				return this->name;
			}

			virtual const char * c_str() const = 0;

		};

		/// @brief Comando SQL já preparado.
		class Query {
		private:

#ifdef DEBUG
			string text;
#endif // DEBUG

			/// @brief Statement a enviar para o banco.
			cppdb::statement st;

			/// @brief Nome das propriedades para o SQL.
			std::vector<string> names;

		public:
			Query(cppdb::session &sql, const XMLNode &node);
			~Query();

			/// @brief Armazena valores processados no banco.
			void store(cppdb::session &sql, const Parser &parser);

		};

		/// &brief Parsers diversos.
		namespace Parser {

		    /// @brief Base para parsers.
		    class Abstract {
            private:

                /// @brief "Pai" desse parser.
                const Abstract *parent;

                /// @brief Lista de valores extraídos do documento.
                std::vector<Content *> contents;

                /// @brief Comandos SQL a executar.
                std::vector<Query *> queryes;

            protected:
                Abstract(cppdb::session &sql, const XMLNode &node, const Abstract *parent = nullptr);
                virtual ~Abstract();

                const char * operator [](const char *name) const;

		    };

		    /// @brief Parser para documento.
		    class Document : public Abstract {
            private:

                /// @brief Lista de filtros para verificar se é um documento válido.
                std::vector<Filter *> filters;

            public:
                Document(cppdb::session &sql, const XMLNode &node);
                virtual ~Document();

                /// @brief Faz o parse do documento.
                bool set(cppdb::session &sql, const Document &document);

		    };


		}

		/*
		/// @brief Parser de documento.
		class Parser {
		private:



		public:

			Parser();
			~Parser();



		};
		*/


	}


#endif // PDFIMPORPER_H_INCLUDED
