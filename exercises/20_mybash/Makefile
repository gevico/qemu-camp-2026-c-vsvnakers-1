# =============================================
# 主 Makefile
# 功能：
#   - mybash：生成二进制，放到 bin/
#   - 其他模块（mysed, myfile, mywc, mytrans）：生成静态库 .a，放到 obj/xxx/，.o 放到 obj/xxx/（或 out/，但建议统一）
#   - 所有子模块 Makefile 应支持 OBJDIR 和 BINDIR 参数
# =============================================

PWD := $(shell pwd)
MAKE_SUB := $(MAKE) --no-print-directory -s

# 所有子模块目录（静态库：生成 .a）1
SUBDIRS := \
	mysed \
	myfile \
	mywc \
	mytrans

# 默认目标：构建 mybash（依赖其他库）
all: mybash

# 通用规则：构建每个子模块（静态库）
$(SUBDIRS):
	@$(MAKE_SUB) -C src/$@ OBJDIR=$(PWD)/obj/$@ BINDIR=$(PWD)/bin

# 构建 mybash（可执行文件，依赖上面的库）
mybash: $(SUBDIRS)
	@$(MAKE_SUB) -C src/mybash OBJDIR=$(PWD)/obj/mybash BINDIR=$(PWD)/bin LINKLIBS="-L$(PWD)/obj/mysed -L$(PWD)/obj/mywc -L$(PWD)/obj/mytrans -L$(PWD)/obj/myfile"

# 清理所有生成的文件
clean:
	@$(MAKE_SUB) -C src/mybash clean OBJDIR=$(PWD)/obj/mybash BINDIR=$(PWD)/bin
	@$(MAKE_SUB) -C src/myfile clean OBJDIR=$(PWD)/obj/myfile BINDIR=$(PWD)/bin
	@$(MAKE_SUB) -C src/mysed clean OBJDIR=$(PWD)/obj/mysed BINDIR=$(PWD)/bin
	@$(MAKE_SUB) -C src/mytrans clean OBJDIR=$(PWD)/obj/mytrans BINDIR=$(PWD)/bin
	@$(MAKE_SUB) -C src/mywc clean OBJDIR=$(PWD)/obj/mywc BINDIR=$(PWD)/bin
	@rm -rf $(PWD)/bin $(PWD)/obj

.PHONY: all $(SUBDIRS) mybash clean