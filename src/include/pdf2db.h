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
	using std::cout;
	using std::endl;

	extern char * chomp(char *str) noexcept;
	extern char * chug (char *str) noexcept;
	extern char * strip(char *str) noexcept;
	extern string & strip(string &str) noexcept;
	extern bool hasSuffix(const char *str, const char *suffix) noexcept;

	namespace PDFImporter {

		extern bool verbose;

		class Query;

		/// @brief Documento PDF já convertido para texto sem espaços.
		class Document {
		private:

			class Page : public std::vector<string> {
			private:
				friend class PDFImporter::Document;

			public:
				Page() = default;

				void get(const unsigned int line, std::vector<string> &text) const;

			};

			std::vector<Page> pages;

		public:
			Document(const char *name);

			bool forEach(std::function<bool(const char *line)> callback) const;

			const string & getString(unsigned int page, unsigned int line) const;

			inline const char * get(unsigned int page, unsigned int line) const {
				return getString(page,line).c_str();
			}

			void get(const unsigned int page, const unsigned int line, std::vector<string> &text) const;

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
		class Property {
		private:

			/// @brief Nome da propriedade.
			string name;

			/// @brief Formato dos dados
			enum Format {
				text,
				timestamp
			} type;

			/// @brief De que página será extraída (0=Todas)?
			unsigned int page;

			/// @brief De que linha será extraída (0=Todas)?
			unsigned int line;

			/// @brief Pego só uma parte da linha?
			struct {
				size_t from;	/// @brief Extrai a partir de qual caractere?
				size_t length;	/// @brief Quantos caracteres?
			} range;

			/// @brief Valor da propriedade.
			string value;

			/// @brief Formatos para conversão.
			struct {

				/// @brief Máscara para conversão de valor de entrada.
				string input;

				/// @brief Máscara para geração do valor de saída.
				string output;

			} format;

		protected:

			/// @brief Define o valor da propriedade.
			void setValue(const char *value);

			inline void setValue(const string &value) {
				setValue(value.c_str());
			}

			/// @brief Cria uma propriedade.
			Property(const XMLNode &node);

		public:
			virtual ~Property() { }

			/// @brief Constrói propriedade pelo nó XML.
			static Property * create(const XMLNode &node);

			/// @brief Obtêm o nome do valor
			const string & getName() const {
				return this->name;
			}

			inline const char * c_str() const {
				return value.c_str();
			}

			inline unsigned int getPage() const noexcept {
				return page;
			}

			inline unsigned int getLine() const noexcept {
				return line;
			}

			inline void clear() noexcept {
				value.clear();
			}

			void prepare(std::vector<string> &text);

			virtual void set(std::vector<string> &text) = 0;

		};

		/// &brief Parsers diversos.
		namespace Parser {

		    /// @brief Base para parsers.
		    class Abstract {
            private:

                /// @brief "Pai" desse parser.
                const Abstract *parent;

            protected:

                /// @brief Comandos SQL a executar.
                std::vector<Query *> queryes;

                /// @brief Meus filhos
                std::vector<Abstract *> childs;

                /// @brief Lista de valores extraídos do documento.
                std::vector<Property *> properties;

                Abstract(cppdb::session &sql, const XMLNode &node, const Abstract *parent = nullptr);
                virtual ~Abstract();

                void store(cppdb::session &sql);

			public:
                const char * operator [](const char *name) const;

                /// @brief Faz o parse do documento.
                virtual bool set(cppdb::session &sql, const PDFImporter::Document &document);

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
                bool set(cppdb::session &sql, const PDFImporter::Document &document) override;

		    };


		}

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
			virtual ~Query();

			/// @brief Armazena valores processados no banco.
			virtual void exec(cppdb::session &sql, const Parser::Abstract &parser);

			/// @brief Obtêm valor inserido
			inline long long getID() {
				return st.last_insert_id();
			}

		};


	}


#endif // PDFIMPORPER_H_INCLUDED
