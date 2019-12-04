@ECHO off

ECHO Inizio pulizia gradlew...

START /WAIT CMD /C  "CD %cd%\android & CALL gradlew.bat clean"
ECHO Fine pulizia gradlew.

pause

EXIT