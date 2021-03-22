# QtXML
Provide XML read/write operation class based on Qt4.8 DOM. 
Drag an xml file into UI and display it via QTreeWidget. 


V1.0 2020-Mar-19
1. Class QtXmlOperation provides read/write xml elements by tag
2. The tag format support nesting such as "root/parent/child", use any sequence of non-word characters as the separator
3. Support drag an xml file into UI to parse it into QTreeWidget