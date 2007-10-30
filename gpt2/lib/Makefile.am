EXTRA_DIST = base.gpt

install:
	mkdir -p $(libdir)/gpt
	cp $(top_srcdir)/lib/base.gpt $(libdir)/gpt
	@echo "============================================================="
	@echo "  Adicione à variável de ambiente GPT_INCLUDE o arquivo      "
	@echo "                                                             "
	@echo "  $(libdir)/gpt/base.gpt                                     "
	@echo "                                                             "
	@echo "  caso deseje utilizar suas funções.                         "
	@echo "============================================================="

uninstall:
	rm -f $(libdir)/gpt/base.gpt