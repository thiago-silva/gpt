#!/bin/sh

function convert
{
  echo "converting ${1}"
  iconv -f iso-8859-1 -t utf-8 $1  > .itmp
  cat .itmp > $1
  rm .itmp
}

for line in `find src -type f | gawk ' !/.svn/ && /[ch]pp$|h$|g$/  { print $1 }'`; do    
    convert $line
done

convert 'README'
convert 'INSTALL'
convert 'NEWS'
convert 'exemplos/highlight.gpt'
convert 'exemplos/olamundo.gpt'
convert 'lib/base.gpt'
