@echo off
for /R %%s in (*.vcproj,*.vcproj.*,*.vcxproj,*.vcxproj.*,*.idb,*.pdb,*.sdf,*.suo,*.ncb,*.sln) do ( 
	echo %%s 
	del /F /Q %%s
)
pause
