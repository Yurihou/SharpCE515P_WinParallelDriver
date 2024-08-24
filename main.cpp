#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    HANDLE     hPrinter;
    DWORD      dwBytesWritten;
}PRINTER;

BOOL startPrinter(PRINTER *printer, LPSTR szPrinterName)
{
    DOC_INFO_1 DocInfo;
    // Need a handle to the printer.
    if(!OpenPrinter( szPrinterName, &(printer->hPrinter), NULL))
        return FALSE;

    // Fill in the structure with info about this "document."
    DocInfo.pDocName = "My Document";
    DocInfo.pOutputFile = NULL;
    DocInfo.pDatatype = "RAW";
    // Inform the spooler the document is beginning.
    if(StartDocPrinter(printer->hPrinter, 1, (LPBYTE)&(DocInfo)) == 0)
    {
        ClosePrinter(printer->hPrinter);
        return FALSE;
    }
    // Start a page.
    if(!StartPagePrinter(printer->hPrinter))
    {
        EndDocPrinter(printer->hPrinter);
        ClosePrinter(printer->hPrinter);
        return FALSE;
    }
    return TRUE;
}

BOOL RawDataToPrinter(PRINTER *printer, LPBYTE lpData, DWORD dwCount)
{
    // Send the data to the printer.
    if(!WritePrinter(printer->hPrinter, lpData, dwCount, &(printer->dwBytesWritten)))
    {
        EndPagePrinter(printer->hPrinter);
        EndDocPrinter(printer->hPrinter);
        ClosePrinter(printer->hPrinter);
        return FALSE;
    }
    return TRUE;
}

BOOL stopPrinter(PRINTER *printer)
{
    // End the page.
    if(!EndPagePrinter(printer->hPrinter))
    {
        EndDocPrinter(printer->hPrinter);
        ClosePrinter(printer->hPrinter);
        return FALSE;
    }
    // Inform the spooler that the document is ending.
    if(!EndDocPrinter(printer->hPrinter))
    {
        ClosePrinter(printer->hPrinter);
        return FALSE;
    }
    // Tidy up the printer handle.
    ClosePrinter(printer->hPrinter);
    // Check to see if correct number of bytes were written.
    //if(printer.dwBytesWritten != printer.dwCount)
    //    return FALSE;
    return TRUE;
}

int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        printf("This is Sharp CE-515P plotter driver.\nThe command format is\n");
        printf("\tce515p [printer name] [command text file]\n");
        printf("Where\n\t[printer name] is the plotter name in your computer;\n");
        printf("\t[command text file] is the plotter command file.\n");
        printf("Detailed information can be seen in document file.\n");
    }
    else if(argc != 3)
    {
        printf("Parameter Error.\nPlease run \"ce515p\" to get help\n");
    }
    else
    {
        PRINTER printer;
        char *printer_name=argv[1];
        BOOL aaa = startPrinter(&printer, printer_name);
        if(aaa == FALSE)
        {
            printf("Plotter \"%s\" not found", printer_name);
            return -1;
        }

        unsigned char cmd_graphmode[3]={0x1b,0x62,0x0d};
        RawDataToPrinter(&printer, cmd_graphmode, 3);

        FILE *fp;
        fp=fopen(argv[2],"r");
        if(fp==NULL)
        {
            printf("Command File \"%s\" not found.\n", argv[2]);
            return -1;
        }

        char cmd_buffer[1000]="";
        while(!feof(fp))
        {
            fgets(cmd_buffer,200,fp);
            int len = strlen(cmd_buffer);
            //printf("%s",cmd_buffer);
            if(cmd_buffer[0]=='E')cmd_buffer[0]=0x1b;
            cmd_buffer[len-1]=0x0d;
            RawDataToPrinter(&printer, (unsigned char*)cmd_buffer, len);
        }
        fclose(fp);
        stopPrinter(&printer);
    }
    return 0;
}
