#ifndef LOCALE_H
#define LOCALE_H

#ifndef _LOCALIZED_
#define _LOCALE_ENG_
#endif

#ifdef _LOCALE_ENG_
#define _RUOTA_VERSION_LONG_ "Ruota " _RUOTA_VERSION_ "\nCopyright 2020 - BSD 3-Clause License\nAuthors:\n * Benjamin Park / parkbenjamin2@gmail.com"
#define _RUOTA_INTERPRETER_START_ "Ruota " _RUOTA_VERSION_ " Interpreter"
#define _STANDARD_LIBRARY_LOADED_ "Standard Library Loaded"
#define _STANDARD_LIBRARY_LOAD_FAIL_ "Failed to load Standard Library: "
#define _FAILURE_FILEPATH_ "Cannot find path to file: "
#define _OPTION_NO_STD_ "Option --no-std (-ns) used; Standard Library not loaded"
#define _UNDECLARED_VARIABLE_ERROR_ "Variable `%1%` is not declared within scope"
#define _INVALID_OBJECT_TYPE_ "Invalid Object type"
#define _UNKNOWN_BUILT_CALL_ "Unknown built in function: %1%"
#define _UNKNOWN_BINARY_OP_ "Unknown binary operator: %1%"
#define _UNKNOWN_UNARY_OP_ "Unknown unary operator: %1%"
#define _FAILURE_INSTANTIATE_OBJECT_ "Cannot instantiate a non-struct Object"
#define _OPERATOR_UNDECLARED_TYPE_ "Operator `%1%` is undefined for value type"
#define _CANNOT_ENTER_DICTIONARY_ "Cannot enter Dictionary with given value"
#define _CANNOT_INDEX_OBJECT_ "Cannot index a non-static, non-instantiated Object"
#define _CANNOT_INDEX_VALUE_ "Value is not an index-able type"
#define _OPERATOR_UNDEFINED_DIFFERENT_TYPE_ "Operator is undefined for base values of different types"
#define _EXTERN_NOT_DEFINED_ "External function `%1%` is not defined"
#define _FAILURE_LENGTH_ "Cannot get length of value"
#define _FAILURE_SIZE_ "Cannot get size of value"
#define _FAILURE_EXTEND_ "Cannot extend a statically declared Object"
#define _FAILURE_STR_TO_NUM_ "String `%1%` cannot be converted to Number"
#define _FAILURE_CONVERT_ "Cannot convert to given type"
#define _FAILURE_ALLOC_ "Cannot initialize a Vector with size < 0"
#define _FAILURE_TO_STR_ "Cannot convert values into String"
#define _NOT_NUMBER_ "Value is not of type `Number`"
#define _NOT_POINTER_ "Value is not of type `Pointer`"
#define _NOT_DICTIONARY_ "Value is not of type `Dictionary`"
#define _NOT_VECTOR_ "Value is not of type `Vector`"
#define _NOT_STRING_ "Value is not of type `String`"
#define _NOT_BOOLEAN_ "Value is not of type `Boolean`"
#define _NOT_OBJECT_ "Value is not of type `Object`"
#define _NOT_TYPE_ "Value is not of type `Type`"
#define _NOT_FUNCTION_ "Value is not of type `Function`"
#define _FUNCTION_VALUE_NOT_EXIST_ "Function does not exist for given value type"
#define _FUNCTION_ARG_SIZE_FAILURE_ "Function overloads do not encompass given argument size"
#define _INDEX_OUT_OF_BOUNDS_ "Index out of bounds: size %1%, got %2%"
#define _FAILURE_PARSE_CODE_ "Failure to parse code"
#define _EXPECTED_ERROR_ "Expected `%1%`"
#define _EXPECTED_FUNCTION_PARAM_ "Expected Function parameter"
#define _EXPECTED_BASE_TYPE_ "Expected base type for prototypic Function declaration"
#define _EXPECTED_FUNCTION_NAME_ "Expected Function name"
#define _EXPECTED_FUNCTION_SIG_ "Expected Function signature"
#define _EXPECTED_OBJECT_NAME_ "Expected Object declaration"
#define _EXPECTED_BASE_CAST_ "Expected base type for value casting"
#define _EXPECTED_RH_ "Expected right-hand expression"
#define _UNEXPECTED_TERMINATION_ "Expression terminated unexpectedly"
#define _NOT_UNARY_OP_ "Operator is not applicable to unary operation"
#define _VALUE_KEY_PAIR_ERROR_ "Value cannot be used to generate key-value pair"
#define _EXPECTED_AFTER_ "Expected expression after `%1%`"
#define _MUST_BE_CONST_ "Values in switch cases must be constant"
#define _EXPECTED_IDF_ "Expected identifier"
#define _EXPECTED_EXPR_ "Expected expression"
#define _EXPECTED_FILE_ "Expected filepath after `load`"
#define _STACK_TRACE_MORE_ " ... (%1% more) ..."
#endif

#ifdef _LOCALE_ITA_
#define _RUOTA_VERSION_LONG_ "Ruota " _RUOTA_VERSION_ "\Diritto d'autore 2020 - BSD 3-Clause Licenza\nAutori:\n * Benjamin Park / parkbenjamin2@gmail.com"
#define _RUOTA_INTERPRETER_START_ "Ruota " _RUOTA_VERSION_ " Interprete"
#define _STANDARD_LIBRARY_LOADED_ "Libreria Standard è Caricata"
#define _STANDARD_LIBRARY_LOAD_FAIL_ "Caricamento di Libreria Standard è Bocciato: "
#define _FAILURE_FILEPATH_ "Non si può trova il percorso del file: "
#define _OPTION_NO_STD_ "Opzione --no-std (-ns) è usata; Libreria Standard non è Caricata"
#define _UNDECLARED_VARIABLE_ERROR_ "Variabile `%1%` non si declarava nell'ambito"
#define _INVALID_OBJECT_TYPE_ "Tipo d'Oggetto (Object) è irrito"
#define _UNKNOWN_BUILT_CALL_ "Funzione predefinita è sconosciuta: %1%"
#define _UNKNOWN_BINARY_OP_ "Operatore binario è sconosciuto: %1%"
#define _UNKNOWN_UNARY_OP_ "Operatore unario è sconosciuto: %1%"
#define _FAILURE_INSTANTIATE_OBJECT_ "Non si replica un Oggetto (Object) che si non definisce con `struct`"
#define _OPERATOR_UNDECLARED_TYPE_ "L'operatore `%1%` è sconosciuto per il tipo di valore dato"
#define _CANNOT_ENTER_DICTIONARY_ "Non si può entrare nel'un Diccionario (Dictionary) col valore dato"
#define _CANNOT_INDEX_OBJECT_ "Non si può indicizzare un Oggetto (Object) che si non definisce con `static`, o non è replcato"
#define _CANNOT_INDEX_VALUE_ "Valore è un tipo che non si può indicizzare"
#define _OPERATOR_UNDEFINED_DIFFERENT_TYPE_ "L'operatore è sconosciuto per valori basici con tipi vari"
#define _EXTERN_NOT_DEFINED_ "Funzione straniera `%1%` non è definita"
#define _FAILURE_LENGTH_ "Valore non ha lunghezza"
#define _FAILURE_SIZE_ "Valore non ha taglia"
#define _FAILURE_EXTEND_ "Non si può stendere un Oggetto (Object) che si declara con `static`"
#define _FAILURE_STR_TO_NUM_ "Non si può convertire Stringa (String) `%1%` a Numero (Number)"
#define _FAILURE_CONVERT_ "Non si può convertire nel tipo dato"
#define _FAILURE_ALLOC_ "Non si può creare un Vettore (Vector) colla lunghezza meno di 0"
#define _FAILURE_TO_STR_ "Non si può convertire i valori in una Stringa (String)"
#define _NOT_NUMBER_ "Il tipo di valore non è `Numero` (Number)"
#define _NOT_POINTER_ "Il tipo di valore non è `Puntatore` (Pointer)"
#define _NOT_DICTIONARY_ "Il tipo di valore non è `Diccionario` (Dictionary)"
#define _NOT_VECTOR_ "Il tipo di valore non è `Vettore` (Vector)"
#define _NOT_STRING_ "Il tipo di valore non è `Stringa` (String)"
#define _NOT_BOOLEAN_ "Il tipo di valore non è `Booleano` (Boolean)"
#define _NOT_OBJECT_ "Il tipo di valore non è `Oggetto` (Object)"
#define _NOT_TYPE_ "Il tipo di valore non è `Tipo` (Type)"
#define _NOT_FUNCTION_ "Il tipo di valore non è `Funzione` (Function)"
#define _FUNCTION_VALUE_NOT_EXIST_ "Non c'è Funzione (Function) scritta per il tipo di valore dato"
#define _FUNCTION_ARG_SIZE_FAILURE_ "Sovraccarichi della Funzione (Function) non consentono la taglia delgi parametri"
#define _INDEX_OUT_OF_BOUNDS_ "Indice è fuori dello scopo: lunghezza di %1%, ricevuto %2%"
#define _FAILURE_PARSE_CODE_ "Processamento è Bocciato"
#define _EXPECTED_ERROR_ "Si aspettava di `%1%`"
#define _EXPECTED_FUNCTION_PARAM_ "Si aspettava di parametri della Funzione (Function)"
#define _EXPECTED_BASE_TYPE_ "Si aspettava tipo basico nella declarazione d'una Funzione (Function) prototipica"
#define _EXPECTED_FUNCTION_NAME_ "Si aspettava il nome della Funzione (Function)"
#define _EXPECTED_FUNCTION_SIG_ "Si aspettava il signatorio della Funzione (Function)"
#define _EXPECTED_OBJECT_NAME_ "Si aspettava la declarazione d'un Oggetto (Object)"
#define _EXPECTED_BASE_CAST_ "Conversione di tipo richiede tipi basici"
#define _EXPECTED_RH_ "Si aspettava un espressione alla destra"
#define _UNEXPECTED_TERMINATION_ "L'espression terminava inaspettatamente"
#define _NOT_UNARY_OP_ "L'operatore non è applicabile a una operazione unaria"
#define _VALUE_KEY_PAIR_ERROR_ "Nella creazione d'un paio chiave-valore non può usare il valore dato"
#define _EXPECTED_AFTER_ "Si aspettava espressione dopo `%1%`"
#define _MUST_BE_CONST_ "I valori nell'espressione `switch` si devono esser' immutabile"
#define _EXPECTED_IDF_ "Si aspettava identificatore"
#define _EXPECTED_EXPR_ "Si aspettava espressione"
#define _EXPECTED_FILE_ "Si aspettava un percorso dopo `load`"
#define _STACK_TRACE_MORE_ " ... (%1% più) ..."
#endif

#ifdef _LOCALE_LAT_
#define _RUOTA_VERSION_LONG_ "Ruota " _RUOTA_VERSION_ "\nDirectus Auctoris ex MMXX - Licentia BSD III-Ideae\nAuctores:\n * Beniamin Parchus / parkbenjamin2@gmail.com"
#define _RUOTA_INTERPRETER_START_ "Ruota " _RUOTA_VERSION_ " Interpretatrum"
#define _STANDARD_LIBRARY_LOADED_ "Bibliotheca Generalis Legitur"
#define _STANDARD_LIBRARY_LOAD_FAIL_ "Bibliotheca Generalis non legi poterat: "
#define _FAILURE_FILEPATH_ "Ad scapum via non legi poterat: "
#define _OPTION_NO_STD_ "Optionem --no-std (-ns) dabatur; Generalis Bibliotheca non legitur"
#define _UNDECLARED_VARIABLE_ERROR_ "Variabilis qui `%1%` nominatur non in dominium declaratur"
#define _INVALID_OBJECT_TYPE_ "Declaratio Structurae [Object] irrita est"
#define _UNKNOWN_BUILT_CALL_ "Appellatio procedurae compilatae ignota est: %1%"
#define _UNKNOWN_BINARY_OP_ "Iussum binaris ignotum est: %1%"
#define _UNKNOWN_UNARY_OP_ "Iussum unaris ignotum est: %1%"
#define _FAILURE_INSTANTIATE_OBJECT_ "Structuram [Object] quae cum `struct` non declaratur non fieri potest"
#define _OPERATOR_UNDECLARED_TYPE_ "Iussum `%1%` pro genus scriptum non est"
#define _CANNOT_ENTER_DICTIONARY_ "Non potes in Matrice [Dictionary] cum valore dato inire"
#define _CANNOT_INDEX_OBJECT_ "Non potes in Structura [Object] quae et cum `static` non declaratur et non ipsa declarata est intrare"
#define _CANNOT_INDEX_VALUE_ "Sicut genus quod intrare non potes declaratus est valor"
#define _OPERATOR_UNDEFINED_DIFFERENT_TYPE_ "Pro genera simplicia et incongruentia irritum est iussum"
#define _EXTERN_NOT_DEFINED_ "Procedura extraria quae `%1%` nominatur non scribitur"
#define _FAILURE_LENGTH_ "Valor longitudonem non habet"
#define _FAILURE_SIZE_ "Valor magnitudonem non habet"
#define _FAILURE_EXTEND_ "A Structura [Object] quae cum `static` declaratur novum non progigni potes"
#define _FAILURE_STR_TO_NUM_ "Series [String] `%1%` in Numerum mutari non potest"
#define _FAILURE_CONVERT_ "Ad genus non mutari potest"
#define _FAILURE_ALLOC_ "Tabula [Vector] cum magnitudone < 0 non creari potest"
#define _FAILURE_TO_STR_ "Valores in Seriem [String] mutari non possunt"
#define _NOT_NUMBER_ "Cum genere `Numerus` [Number] non declaratur valor"
#define _NOT_POINTER_ "Cum genere `Index` [Pointer] non declaratur valor"
#define _NOT_DICTIONARY_ "Cum genere `Matrix` [Dictionary] non declaratur valor"
#define _NOT_VECTOR_ "Cum genere `Tabula` [Vector] non declaratur valor"
#define _NOT_STRING_ "Cum genere `Series` [String] non declaratur valor"
#define _NOT_BOOLEAN_ "Cum genere `Logicum` [Boolean] non declaratur valor"
#define _NOT_OBJECT_ "Cum genere `Structura` [Object] non declaratur valor"
#define _NOT_TYPE_ "Cum genere `Genus` [Type] non declaratur valor"
#define _NOT_FUNCTION_ "Cum genere `Procedura` [Function] non declaratur valor"
#define _FUNCTION_VALUE_NOT_EXIST_ "Pro valorem datum non est Procedura [Function]"
#define _FUNCTION_ARG_SIZE_FAILURE_ "Pro magnitudonem parametrorum datorum scipta non est Procedura [Function]"
#define _INDEX_OUT_OF_BOUNDS_ "Postulum datum extra magnitudonem est: habet %1%, %2% dabatur"
#define _FAILURE_PARSE_CODE_ "Compilatio processi defecit"
#define _EXPECTED_ERROR_ "`%1%` exspectabatur"
#define _EXPECTED_FUNCTION_PARAM_ "Parametrum Procedurae [Function] exspectabatur"
#define _EXPECTED_BASE_TYPE_ "Genus simplex in declaratio Procedurae [Function] successae exspectabatur"
#define _EXPECTED_FUNCTION_NAME_ "Nomen Procedurae [Function] exspectabatur"
#define _EXPECTED_FUNCTION_SIG_ "Signum Procedurae [Function] exspectabatur"
#define _EXPECTED_OBJECT_NAME_ "Declaratio Structurae [Object] exspectabatur"
#define _EXPECTED_BASE_CAST_ "Genus simplex in iusso mutationis exspectabatur"
#define _EXPECTED_RH_ "In dextro enuntiatio exspectabatur"
#define _UNEXPECTED_TERMINATION_ "Ad finem suam adtigit enuntiatio de improviso"
#define _NOT_UNARY_OP_ "In iusso unare non agi potest"
#define _VALUE_KEY_PAIR_ERROR_ "In coniugium Matrici [Dictionary] non accedere potest valor"
#define _EXPECTED_AFTER_ "Post `%1%` enuntiatio exspectabatur"
#define _MUST_BE_CONST_ "Valores qui in enuntiatione `switch` sunt debent declarari esse incommutabiles"
#define _EXPECTED_IDF_ "Nomen exspectabatur"
#define _EXPECTED_EXPR_ "Enuntiatio exspectabatur"
#define _EXPECTED_FILE_ "Post `load` ad scapum via exspectabatur"
#define _STACK_TRACE_MORE_ " ... (plus %1%) ..."
#endif

#ifdef _LOCALE_JPN_
#define _RUOTA_VERSION_LONG_ "ルオタ・" _RUOTA_VERSION_ "\n2020版権・BSD 3-Clause License\n作者:\n * パーク・ベンジャミン / parkbenjamin2@gmail.com"
#define _RUOTA_INTERPRETER_START_ "ルオタ・" _RUOTA_VERSION_ "のインタプリタ"
#define _STANDARD_LIBRARY_LOADED_ "一般的なライブラリーはロードされた"
#define _STANDARD_LIBRARY_LOAD_FAIL_ "一般的なライブラリーのロードは駄目："
#define _FAILURE_FILEPATH_ "ファイルの道は不明："
#define _OPTION_NO_STD_ "「--no-stdか-ns」は使われた・一般的なライブラリーはされなかった"
#define _UNDECLARED_VARIABLE_ERROR_ "変数の「%1%」は有効範囲の中で見られない"
#define _INVALID_OBJECT_TYPE_ "クラス（Object）の種類は無効"
#define _UNKNOWN_BUILT_CALL_ "先天の関数の「%1%」は不明"
#define _UNKNOWN_BINARY_OP_ "二項の演算子の「%1%」は不明"
#define _UNKNOWN_UNARY_OP_ "一項の演算子の「%1%」は不明"
#define _FAILURE_INSTANTIATE_OBJECT_ "「struct」で出来なかったクラス（Object）の実体化は駄目"
#define _OPERATOR_UNDECLARED_TYPE_ "値のために演算子の「%1%」は定義しなかった"
#define _CANNOT_ENTER_DICTIONARY_ "くれた値を使ってテーブル（Dictionary）の入りは駄目"
#define _CANNOT_INDEX_OBJECT_ "「struct」で出来なかったか実体化されなかったらクラス（Object）の入りはダメ"
#define _CANNOT_INDEX_VALUE_ "入られラれない種類として値は定義した"
#define _OPERATOR_UNDEFINED_DIFFERENT_TYPE_ "違っている値の種類のために演算子は駄目"
#define _EXTERN_NOT_DEFINED_ "外の関数の「%1%」は不明"
#define _FAILURE_LENGTH_ "値は長さがない"
#define _FAILURE_SIZE_ "値は大きさがない"
#define _FAILURE_EXTEND_ "「static」で出来たクラス（Object）を土台として使うのは駄目"
#define _FAILURE_STR_TO_NUM_ "文字列（String）の「%1%」は数にするのは駄目"
#define _FAILURE_CONVERT_ "値の変換は駄目"
#define _FAILURE_ALLOC_ "ゼロ以下の長さの同意列（Vector）は駄目"
#define _FAILURE_TO_STR_ "値は文字列の変換は駄目"
#define _NOT_NUMBER_ "値の種類は「数」（Number）ではない"
#define _NOT_POINTER_ "値の種類は「ポインター」（Pointer）ではない"
#define _NOT_DICTIONARY_ "値の種類は「テーブル」（Dictionary）ではない"
#define _NOT_VECTOR_ "値の種類は「同意列」（Vector）ではない"
#define _NOT_STRING_ "値の種類は「文字列」（String）ではない"
#define _NOT_BOOLEAN_ "値の種類は「ブール」（Boolean）ではない"
#define _NOT_OBJECT_ "値の種類は「クラス」（Object）ではない"
#define _NOT_TYPE_ "値の種類は「種類」（Type）ではない"
#define _NOT_FUNCTION_ "値の種類は「関数」（Function）ではない"
#define _FUNCTION_VALUE_NOT_EXIST_ "くれた種類の関数（Function）は不明"
#define _FUNCTION_ARG_SIZE_FAILURE_ "徐変数の大きさの過負荷は関数（Function）に不明"
#define _INDEX_OUT_OF_BOUNDS_ "指数は遠すぎる：大きさは「%1%」、「%2%」を上げた"
#define _FAILURE_PARSE_CODE_ "コードの解析は失敗"
#define _EXPECTED_ERROR_ "「%1%」は思い設けられた"
#define _EXPECTED_FUNCTION_PARAM_ "関数（Function）の徐変数は思い設けられた"
#define _EXPECTED_BASE_TYPE_ "関数（Function）の宣言は基本的な種類を思い設けた"
#define _EXPECTED_FUNCTION_NAME_ "関数（Function）の名は思い設けられた"
#define _EXPECTED_FUNCTION_SIG_ "関数（Function）の署名は思い設けられた"
#define _EXPECTED_OBJECT_NAME_ "クラス（Object）の識別名は思い設けられた"
#define _EXPECTED_BASE_CAST_ "キャストするために基本的な種類は思い設けられた"
#define _EXPECTED_RH_ "右に表現は思い設けられた"
#define _UNEXPECTED_TERMINATION_ "案外に表現が終わった"
#define _NOT_UNARY_OP_ "一項の作動で演算子をするのは駄目"
#define _VALUE_KEY_PAIR_ERROR_ "キーと値の組を作るために使うのは駄目"
#define _EXPECTED_AFTER_ "「%1%」の後で表現は思い設けられた"
#define _MUST_BE_CONST_ "スイッチの中の値は不変ではなくてはいけない"
#define _EXPECTED_IDF_ "識別名は思い設けられた"
#define _EXPECTED_EXPR_ "表現は思い設けられた"
#define _EXPECTED_FILE_ "「load」の後でファイルの道は思い設けられた"
#define _STACK_TRACE_MORE_ " ... （これから%1%以上） ..."
#endif

#endif