<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="es_ES">
<context>
    <name>QUMultilingualTaskFactory</name>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="25"/>
        <source>Unnamed plugin</source>
        <translation>Plugin sin nombre</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="27"/>
        <source>Unknown person</source>
        <translation>Persona desconocida</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="28"/>
        <source>No description given.</source>
        <translation>Sin descripción.</translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTask</name>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="20"/>
        <source>Assign missing files automatically</source>
        <translation>Asignar archivos faltantes automáticamente</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="21"/>
        <source>&lt;b&gt;#MP3&lt;/b&gt; ... first audio file found.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... first video file found.&lt;br&gt;&lt;br&gt;Does not reset tags that have already been set.</source>
        <translation>&lt;b&gt;#MP3&lt;/b&gt; ... primer archivo de audio encontrado.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... primer archivo de imagen, según un patrón.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... primer archivo de imagen, según un patrón.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... primer archivo de video encontrado.&lt;br&gt;&lt;br&gt;No restaura las etiquetas que ya han sido colocadas.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="29"/>
        <source>Fix or remove unsupported tags</source>
        <oldsource>Remove unsupported tags</oldsource>
        <translation type="unfinished">Eliminar etiquetas no soportadas</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="30"/>
        <source>Replaces the incorrect &lt;b&gt;#AUTHOR&lt;/b&gt; tag by the correct &lt;b&gt;#CREATOR&lt;/b&gt; tag and deletes all other unsupported tags.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="34"/>
        <source>Fix audio length via #END tag</source>
        <translation>Fijar duración de la pista de audio con la etiqueta #END</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="35"/>
        <source>Sets &lt;b&gt;#END&lt;/b&gt; to the length of the song if audio file present and longer than song. &lt;b&gt;Appends a little time buffer.&lt;/b&gt;</source>
        <translation>Establece &lt;b&gt;#END&lt;/b&gt; a la duración de la canción si la pista de audio existe y es más larga que la canción. &lt;b&gt;Añade un pequeño buffer de tiempo.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="39"/>
        <source>Round #GAP to nearest integer</source>
        <oldsource>Round #GAP to nearest integer.</oldsource>
        <translation>Redondear #GAP al entero más cercano</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="43"/>
        <source>Capitalize #TITLE tag</source>
        <oldsource>Fix capitalization of #TITLE tag</oldsource>
        <translation>Poner e mayúsculas la etiqueta #TITLE</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="44"/>
        <source>Applies some rules of capitalization to form a consistent appearance for all song titles.&lt;br&gt;&lt;br&gt;&lt;b&gt;Useful for english songs.&lt;/b&gt;</source>
        <translation>Aplica algunas reglas de mayúsculas para conseguir una apariencia consistente entre los títulos de las canciones.&lt;br&gt;&lt;br&gt;&lt;b&gt;Útil para canciones en inglés.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="48"/>
        <source>Capitalize #ARTIST tag</source>
        <translation>Poner en mayúsculas la etiqueta #ARTIST</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="49"/>
        <source>Capitalize each word of the artist, e.g. &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</source>
        <translation>Utiliza mayúsculas en cada palabra del artista, ej.: &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="94"/>
        <location filename="QUPreparatoryTask.cpp" line="95"/>
        <source>Pattern:</source>
        <translation>Patron:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="94"/>
        <source>(cover)</source>
        <translation>(carátula)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="95"/>
        <source>(background)</source>
        <translation>(imagen de fondo)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="102"/>
        <source>Buffer:</source>
        <translation>Buffer:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="102"/>
        <source>seconds</source>
        <translation>segundos</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="105"/>
        <source>Capitalize English songs only</source>
        <translation>Utilizar mayúsculas solo en las canciones en inglés</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="106"/>
        <source>Capitalize each word</source>
        <translation>Utilizar mayúsculas en cada palabra</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="107"/>
        <location filename="QUPreparatoryTask.cpp" line="110"/>
        <source>Capitalize first word only</source>
        <translation>Utilizar mayúsculas solo en la primera palabra</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="149"/>
        <source>Capitalization fix not applicable due to non-english song: &quot;%1 - %2&quot;. Try to configure the task.</source>
        <translation>Uso de mayúsculas no corresponde por no ser una canción en inglés: &quot;%1 - %2&quot;. Intenta configurar la tarea.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="157"/>
        <source>Capitalization fix not applicable due to empty title: &quot;%1&quot;</source>
        <translation>Uso de mayúsculas no corresponde por título vacío: &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="222"/>
        <source>Capitalization of #TITLE fixed for &quot;%1 - %2&quot;</source>
        <translation>Uso de mayúsculas en #TITLE para &quot;%1 - %2&quot;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="235"/>
        <source>Capitalization fix not applicable due to empty artist: &quot;%1&quot;</source>
        <translation>Uso de mayúsculas no corresponde por artista vacío: &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="250"/>
        <source>Capitalization of #ARTIST fixed for &quot;%1 - %2&quot;</source>
        <translation>Uso de mayúsculas en #ARTIST para &quot;%1 - %2&quot;</translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTaskFactory</name>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="11"/>
        <source>Preparatory Tasks</source>
        <translation>Tareas preparatorias</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="15"/>
        <source>Fix general issues and prepare song for other tasks.</source>
        <translation>Arreglar problemas generales y preparar la canción para otras tareas.</translation>
    </message>
</context>
<context>
    <name>QUSimpleTaskFactory</name>
    <message>
        <location filename="../shared/QUSimpleTaskFactory.h" line="16"/>
        <source>Simple Task</source>
        <translation>Tarea sencilla</translation>
    </message>
</context>
<context>
    <name>QUSmartSetting</name>
    <message>
        <location filename="../shared/QUSmartSetting.cpp" line="30"/>
        <source>Reset to default value.</source>
        <translation>Restaurar a valor por defecto.</translation>
    </message>
</context>
</TS>
