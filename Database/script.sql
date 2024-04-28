
-------------------------------------------------------------STOCK------------------------------------------------
CREATE TABLE Stocks
(
	Id bigint not null,
	"Name" varchar(50) not null,
	"Status" varchar(50) not null,
	Quantity_Status varchar(6) not null,
	Quantity int not null,
	Cost_Price decimal(3, 3) not null,
	Sole_Price decimal(3, 3) not null,
	"User" varchar(12) not null,
	MFG_date datetime not null,
	"Description" varchar(300) not null,
	Photo VARBINARY,
	Exp_date datetime not null,
	Supplier varchar(50) not null,
	Supplier_Contact varchar(50) not null,
	"Date Added" datetime,
	constraint pk_Id PRIMARY KEY(Id),
	constraint chk check(Quantity > 0 and Sole_Price > 0 and Cost_Price > 0),
    constraint uniqu_name unique("Name")
)
GO

CREATE INDEX stock_index ON Stocks("Name");--Creating index to facilitate searching of drug name

-- Create a new stored procedure called 'MyStock' in schema 'dbo'
-- Drop the stored procedure if it already exists
IF EXISTS
(
	SELECT * FROM INFORMATION_SCHEMA.ROUTINES
	WHERE SPECIFIC_SCHEMA = N'dbo' AND SPECIFIC_NAME = N'MyStock' AND ROUTINE_TYPE = N'PROCEDURE'
)
DROP PROCEDURE dbo.MyStock
GO

CREATE PROCEDURE MyStock
(
	@Id bigint,
	@Name varchar(50),
	@Status varchar(50),
	@Quantity_Status varchar(6),
	@Quantity int,
	@Cost_Price decimal,
	@Sole_Price decimal,
	@User varchar(12),
	@MFG_date datetime,
	@Exp_date datetime,
	@Description varchar(300),
	@Photo VARBINARY,
	@Supplier varchar(50),
	@Supplier_Contact varchar(50),
	@Date_Added datetime
)AS
BEGIN
	BEGIN TRY
		BEGIN TRANSACTION;

		 DECLARE @stocks nvarchar(1000);
        SET @stocks = N'INSERT INTO Stocks (Id, "Name", Status, Quantity_Status, Quantity, Cost_Price, Sole_Price, User, MFG_date, Exp_date, "Description", Photo, Supplier, Supplier_Contact, "Date Added")
		VALUES (@Id, @"Name", @"Status", @Quantity_Status, @Quantity, @"User", @Cost_Price, @Sole_Price, @"Description", @MFG_date, @Exp_date, @Supplier, @Supplier_Contact, @Photo, @"Date Added")';
		
		COMMIT TRANSACTION;
	END TRY

	BEGIN CATCH
		IF @@TRANCOUNT > 0
			ROLLBACK TRANSACTION;

			DECLARE @error VARCHAR(400) = ERROR_MESSAGE();
			DECLARE @severe INT = ERROR_SEVERITY();
			DECLARE @condition INT = ERROR_STATE();

			RAISERROR (@error, @severe, @condition);
	END CATCH
END
GO
-------------------------------------------------------------ADD USER------------------------------------------------
CREATE TABLE ADD_USER
(
	UserId bigint not null,
	"Name" varchar(50),
	"Status" varchar(12) not null,
	Email varchar(20) not null,
	"Password" varchar(20) not null,
	Photo VARBINARY,
	Issued_Date datetime,
	constraint pk_Idx primary key(Id),
    constraint unique_email unique(Email),
	constraint chk_email_format check(Email like '%_@__%.__%')
);
GO
CREATE INDEX User_Index on ADD_USER("Name");

-- Create a new stored procedure called 'StoredProcedureName' in schema 'dbo'
-- Drop the stored procedure if it already exists
IF EXISTS
(
	SELECT * FROM INFORMATION_SCHEMA.ROUTINES
	WHERE SPECIFIC_SCHEMA = N'dbo' AND SPECIFIC_NAME = N'MyUser' AND ROUTINE_TYPE = N'PROCEDURE'
)
DROP PROCEDURE dbo.StoredProcedureName

GO
CREATE PROCEDURE MyUser
(
	@UserId bigint,
	@Name varchar(50),
	@Status varchar(12),
	@Email varchar(20),
	@Password varchar(20),
	@Photo VARBINARY,
	@Issued_Date datetime
)AS
BEGIN
	BEGIN TRY
		BEGIN TRANSACTION;

		 DECLARE @users nvarchar(1000);
        SET @users = N'INSERT INTO Stocks (Id, "Name", "Status", Email, "Password", Photo, Issued_Date)
		VALUES (@UserId, @Name, @Status, @Email, @Password, @Photo, @Issued_Date)';

		EXEC Sp_executesql @users, N'@UserId, @Name, @Status, @Email, @Password, @Photo, @Issued_Date',
		@UserId, @Name, @Status, @Email, @Password, @Photo, @Issued_Date;

	--	EXEC MyUser @Id = 10005, @Name = 'SmartBrain', @Status = 'Admin', @Email = 'smart@gmail.com', @Password = 'smart@35', @Photo =  null, @Issued_Date = null;

		COMMIT TRANSACTION;
	END TRY

	BEGIN CATCH
		IF @@TRANCOUNT > 0
			ROLLBACK TRANSACTION;

			DECLARE @errors VARCHAR(400) = ERROR_MESSAGE();
			DECLARE @severity INT = ERROR_SEVERITY();
			DECLARE @conditions INT = ERROR_STATE();

			RAISERROR (@errors, @severity, @conditions);

	END CATCH
END
GO

-------------------------------------------------------------SALES------------------------------------------------
CREATE TABLE Sales
(
	BatchCode bigint not null,
	"Name" varchar(50) not null,
	"Status" varchar(50) not null,
	Quantity_Status varchar(6) not null,
	Quantity int not null,
	Sole_Price decimal(3, 3) not null,
	"User" varchar(12) not null,
	"Description" varchar(300) not null,
	MFG_date datetime not null,
	Exp_date datetime not null,
	Photo VARBINARY,
	Issued_Date Datetime,
	Invoice_Id bigint foreign key references Invoice(Invoice_Id),

	constraint pk_Id PRIMARY KEY(BatchCode),
	constraint chk check(Quantity > 0 and Sole_Price > 0),
    constraint uniqu_name unique("Name")
)
GO

CREATE INDEX Sales_index ON Sales(BatchCode);--Creating index to facilitate searching of sales id

-- Create a new stored procedure called 'MySales' in schema 'dbo'
-- Drop the stored procedure if it already exists
IF EXISTS
(
	SELECT * FROM INFORMATION_SCHEMA.ROUTINES
	WHERE SPECIFIC_SCHEMA = N'dbo' AND SPECIFIC_NAME = N'MySales' AND ROUTINE_TYPE = N'PROCEDURE'
)
DROP PROCEDURE dbo.MySales
GO

CREATE PROCEDURE MySales
(
	@BatchCode bigint not null,
	@"Name" varchar(50) not null,
	@"Status" varchar(50) not null,
	@Quantity_Status varchar(6) not null,
	@Quantity int not null,
	@Sole_Price decimal(3, 3) not null,
	@"User" varchar(12) not null,
	@"Description" varchar(300) not null,
	@MFG_date datetime not null,
	@Exp_date datetime not null,
	@Photo VARBINARY,
	@Issued_Date

)AS
BEGIN
	BEGIN TRY
		BEGIN TRANSACTION;

		 DECLARE @sales nvarchar(1000);
        SET @sales = N'INSERT INTO Stocks (BatchCode, "Name", Status, Quantity_Status, Quantity, Sole_Price, User, "Description", MFG_date, Exp_date,  Photo, Issued_Date)
		VALUES (@BatchCode, @"Name", @Status, @Quantity_Status, @Quantity, @Sole_Price, @User, @"Description", @MFG_date, @Exp_date,  @Photo, @Issued_Date)';
		
		COMMIT TRANSACTION;
	END TRY

	BEGIN CATCH
		IF @@TRANCOUNT > 0
			ROLLBACK TRANSACTION;

			DECLARE @error VARCHAR(400) = ERROR_MESSAGE();
			DECLARE @severe INT = ERROR_SEVERITY();
			DECLARE @condition INT = ERROR_STATE();

			RAISERROR (@error, @severe, @condition);
	END CATCH
END
GO

GO
-------------------------------------------------------------Customers------------------------------------------------
CREATE TABLE Customers
(
	Cust_Id bigint identity(1002, 3) not null,
	Cust_Name varchar(20) not null,
	Cust_Phone varchar(13) not null,
	Cust_Address varchar(30) not null,
	constraint pk_Id PRIMARY KEY(Cust_Id),
    constraint uniqu_phone unique(Cust_Phone)
)
GO

-------------------------------------------------------------invoice------------------------------------------------
CREATE TABLE Invoice
(
	Invoice_Id bigint identity(1012, 3) not null,
	Item_Name varchar(20) not null,
	"Status" varchar(50) not null,
	Quantity_Status varchar(6) not null,
	Quantity int not null,
	Sole_Price decimal(3, 3) not null,
	Invoice_Date datetime,
	Customer_Id bigint foreign key references Customers(Cust_Id),
	constraint pk_Id PRIMARY KEY(Cust_Id),
    constraint uniqu_phone unique(Cust_Phone)
)
GO

-------------------------------------------------------------Employee------------------------------------------------
CREATE TABLE Employees
(
	Emp_Id bigint identity(100, 3) not null,
	Emp_Name varchar(20) not null,
	Emp_Phone varchar(13) not null,
	Emp_Address varchar(30) not null,
	Emp_Designation varchar(13) not null,
	Emp_Salary decimal(5, 3),
	Emp_photo varbinary not null,
	UserId foregn key references ADD_USER(UserId)
	constraint pk_EmpployeeId PRIMARY KEY(Emp_Id),
    constraint unique_phone unique(Emp_Phone)
)
GO







--BACKUP DATABASE PHARMACY TO DISK = 'D:\Projects\AppDev\Pharmacy_2\Database\MyBackup.bak' WITH init;
BACKUP DATABASE PHARMACY TO DISK = 'D:\Projects\AppDev\Pharmacy_2\Database\MyBackup.bak' WITH  DIFFERENTIAL;


SELECT * FROM Stocks;