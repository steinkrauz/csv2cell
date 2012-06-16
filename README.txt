+ csv2cell

This program converts a table from a .csv file into a table defined with wikidot.com syntax. If you have MS Excel, you better use other means of automatisation, cause this program will not provide any flexible or rich functionality.

Usage is simple: csv2cell inpute_file output_file
If both filenames or output_file missed, the program will use standard input and output, i.e. it can participate in a command chain.

There is no built-in rules of transformation. One should edit the configuration file, csv2cell.json

Descrition of the config file:
{
  "cell_size": 512,
  Size of the cell buffer, should be big enough to handle the longest text in the table.
 
  "column_count": 4,
  Number of columns in the table. 

  "separator": ";",
  A character to separate cells. The most preferable character to use is a semicolon, as dots, commas and colons are often used in numbers and dates and could confuse the program. 
 
  "control_column": 4,
  A column where a mark can be placed. The mark is an asterisk currently. Any row with the mark in the control column will have a row_style applied. Useful for highlighting something important.

  "row_style": "background-color:rgb(191,244,174);",
  A CSS style for a marked row.

  "cell_style": "border: 1px solid silver;"
  A CSS style for a regular cell.
}

