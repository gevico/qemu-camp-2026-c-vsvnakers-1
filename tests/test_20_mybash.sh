#!/bin/bash
# test_20_mybash.sh
# 运行 mybash 程序并执行命令脚本

# 获取脚本所在目录的绝对路径
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
EX_DIR="$SCRIPT_DIR/../exercises/20_mybash"

# 运行 mybash，执行命令脚本
cd "$EX_DIR"
./bin/mybash "$SCRIPT_DIR/mybash_cmd.sh"
# 这是一个测试改动