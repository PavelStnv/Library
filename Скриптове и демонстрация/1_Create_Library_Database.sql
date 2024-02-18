USE [master]
GO

IF NOT EXISTS (SELECT * FROM sys.databases WHERE physical_database_name = 'Library')
BEGIN
	CREATE DATABASE [Library]
END
GO