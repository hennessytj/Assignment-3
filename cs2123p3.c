/**********************************************************************
Program cs2123p3.c by Timothy Hennessy
Purpose:	
    Program parses a file and uses a linked list to process
    the data as time series events.
Command Parameters:
    No Command Paramters are used in the execution of this 
    program. 
Input:
    This program uses the standard input stream for its
    input (i.e., a file is redirected at the command line).  
    E.g., ./p3 < p3Input.txt
Results:
     Time  Person               Event          
     -----------------------------------          
     0     Fred                 Arrive         
     5     Daphne               Arrive         
     8     Velma                Arrive         
     9     Daphne               Depart         
     10    Fred                 Depart         
     14    Shaggy               Arrive         
     14    Velma                Depart         
     20    Scooby               Arrive         
     22    Scooby               Depart         
     23    Shaggy               Depart         
     25    Scrappy              Arrive         
     26    Scrappy              Depart         
     26    SIMULATION           TERMINATES 
Returns:
    Refer to cs2123p3.h for a full listing
    of various return values.
Notes:
    1.  User must redirect a properly formatted input file
        from command line for successful execution
    2.  The program implements a singly linked list
**********************************************************************/

/* include files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "cs2123p3.h"

// linked list functions - you must provide the code for these (see course notes)
/******************** removeLL *************************************
int removeLL(LinkedList list, Event  *pValue)
Purpose:
    Removes a node from the front of a LinkedList and returns the
    Event structure (via the parameter list).
Parameters:
    I   LinkedList list       pointer to the LinkedList
    I   Event *pValue         value structure of node being removed

Returns:
  Functionally
      TRUE  - LinkedList was not empty
      FALSE - LinkedList was passed in empty
  Parameter
      Event structure containing value of node which was removed

Notes:
  Removes the first node from the linked list.  Returns the nodes event
  value via the parameter list.  Sets list->pHead to list->pHead->pNext.
**************************************************************************/
int removeLL(LinkedList list, Event *pValue)
{
	NodeLL *pRemove;
	if (list->pHead == NULL)
		return FALSE;                   // list was passed in empty
	*pValue = list->pHead->event;       // store Event structure into pValue
	                                    // so it can be returned via param
	pRemove = list->pHead;              // set the address of p to first 
	                                    // node in LinkedList
	list->pHead = list->pHead->pNext;   // new first node in LinkedList
	free(pRemove);
	return TRUE;                        // successfully removed a node
}
/******************** allocateNodeLL *************************************
NodeLL *allocateNodeLL(LinkedList list, Event value)
Purpose:
    Allocates new node for LinkedList.
Parameters:
    I   LinkedList list       pointer to the LinkedList
    I   Event value           values to be given to new node

Returns:
  A pointer to the new dynamically allocated node.

Notes:
  If dynamic allocation returns NULL pointer the program aborts.    
**************************************************************************/
NodeLL *allocateNodeLL(LinkedList list, Event value)
{
	NodeLL *pNew;
	pNew = (NodeLL *) malloc(sizeof(NodeLL));
	if (pNew == NULL)
		ErrExit(ERR_ALGORITHM, "No available memory for linked list");
	pNew->event = value;
	pNew->pNext = NULL;
	return pNew;
}
/******************** searchLL *******************************************
NodeLL *searchLL(LinkedList list, int match, NodeLL **ppPrecedes)
Purpose:
    Uses match (from argument passed in to function via parameter)
    to iterate over linked list and find if a NodeLL contains a matching
    value.
Parameters:
    I   LinkedList list       pointer to the LinkedList
    I   int   match           stores value to be found in LinkedList
    O   NodeLL ppPrecedes     pointer to a pointer to a NodeLL; used 
                              to return the address of the NodeLL found
                              right before the matching NodeLL

Returns:
  Functionally
    NULL    - If a match is not found in LinkedList
    Address - The address of the match in LinkedList
  Parameter
    ppPrecedes - returns address of previous NodeLL
Notes:
                               Values Returned
  Condition                    Functionally         Via Param
  Match at beginning           ppPrecedes = NULL    p = pHead
  Match Less than first Node   ppPrecedes = NULL    p = NULL
  Match between Nodes          ppPrecedes = p       p = p->pNext
  List is empty                ppPrecedes = NULL    p = NULL
  Match is less than           ppPrecedes = p       p = p->pNext
  (between nodes)
**************************************************************************/
NodeLL *searchLL(LinkedList list, int match, NodeLL **ppPrecedes)
{
	NodeLL *pCurrent;
	// used when the list is empty or we need to insert at the beginning
	*ppPrecedes = NULL;
	
	// Traverse through the list loooking for where the key belongs or
	// the end of the list.
	for (pCurrent = list->pHead; pCurrent != NULL; pCurrent = pCurrent->pNext)
	{
		if (match == pCurrent->event.iTime)     // assume match is an integer
			return pCurrent;
		if (match < pCurrent->event.iTime)      // assume the LL is ordered
			return NULL;
		*ppPrecedes = pCurrent;                 // pointer to a NodeLL
	}
	// Not found return NULL
	return NULL;
}
/******************** insertOrderedLL *************************************
NodeLL *insertOrderedLL(LinkedList list, Event value)
Purpose:
    Inserts a NodeLL into its respective place in the ordered
    LinkedList.
Parameters:
    I   LinkedList list       pointer to the LinkedList
    I   Event value           ??

Returns:
  ?

Notes:
  ?
Update so it can add events with the same time value
**************************************************************************/
NodeLL *insertOrderedLL(LinkedList list, Event value)
{
	NodeLL *pNew;
	NodeLL *pFind;
	NodeLL *pPrecedes;
	// see if it already exists
	// pFind is not necessary unless we are concerned
    // about duplicates
	pFind = searchLL(list, value.iTime, &pPrecedes);
	// commented out code to allow for duplicates
	//if (pFind != NULL)
	//	return pFind;
	
	// Allocate a node and insert.
	pNew = allocateNodeLL(list, value);
	
	// Check for inserting at the beginning of the list
	// this will also handle when the list is empty
	if (pPrecedes == NULL)
	{
		pNew->pNext = list->pHead;
		list->pHead = pNew;
	}
	else
	{
		pNew->pNext = pPrecedes->pNext;
		pPrecedes->pNext = pNew;
	}
	return pNew;
}
/******************** newLinkedList **************************************
LinkedList newLinkedList(void)
Purpose:
    Dynamically allocates memory for a new linked list.
Parameters:
    O   LinkedList list       pointer to a LinkedListImp

Returns:
    A pointer to a LinkedListImp structure (the pointer is called 
    list) that has a pointer (called pHead) to a NodeLL initialized to NULL.
Notes:
    ?
**************************************************************************/
LinkedList newLinkedList()
{
	LinkedList list = (LinkedList) malloc(sizeof(LinkedListImp));
	
	if (list == NULL)
		ErrExit(ERR_ALGORITHM, "No available memory for linked list");
	// Mark the list as empty
	list->pHead = NULL;      // empty list
	return list;
}
// end of functions from notes
// begin program 3 functions
/******************** addEventNodes *******************************************************
void addEventNodes(LinkedList list, char szPersonName[], int iDepUnits, int iArriveTime)
Purpose:
    Takes input from stdin and adds it to list.
Parameters:
    I  LinkedList list       pointer to a LinkedListImp
    I  char szName[]         contains name of person
    I  int iDepUnits         number of clock units until person departs
    I  int iArriveTime       time person arrives

Returns:
    N/A
Notes:
    Assigns event structure with initial arrival data and adds it to linked list.
    Next, the event structure is updated with departure data and added to
    the linked list.
*******************************************************************************************/
void addEventNodes(LinkedList list, char szPersonName[], int iDepUnits, int iArriveTime)
{
	Event addEvent;
	
    // add data to addEvent structure before inserting into Linked List
	addEvent.iTime = iArriveTime;            
	addEvent.iEventType = EVT_ARRIVE;             // set event type to Arrive
	strcpy(addEvent.person.szName, szPersonName); // store name of person
	addEvent.person.iDepartUnits = iDepUnits;

	// insert Arrive Event into Linked List
	insertOrderedLL(list, addEvent);
	
	// update addEvent structure before inserting into Linked List
	addEvent.iEventType = EVT_DEPART;
	addEvent.iTime = iArriveTime + iDepUnits; 
	
	// insert Depart Event into Linked List
	insertOrderedLL(list, addEvent);
}
/******************** readEventData **************************************
int readEventData(LinkedList list)
Purpose:
    Takes input from stdin and adds it to list.
Parameters:
    I  LinkedList list       pointer to a LinkedListImp

Returns:
    Returns a time corresponding to the end of events
Notes:
    N/A
**************************************************************************/
int readEventData(LinkedList list)
{
	char szInputBuffer[MAX_LINE_SIZE];    // entire input line
	int iCurrentArriveTime = 0;           // used to store current
                                          // arrive event times
	int iNextArrivalTime;                 // stores next arrival
	int iDepartureUnits = 0;              // store depart units
	char *pszRemainingTxt;                // pointer to next token
	Token szToken;                        // stores token returned
	                                      // via param from getToken
	int iScanfCnt;                        // successful converisons
	
	// take a line from stdin
	// e.g., Fred 10 5 or Daphne 4 3 etc...
	while(fgets(szInputBuffer, MAX_LINE_SIZE, stdin) != NULL)
	{
		if (szInputBuffer[0] == '\n')
			continue;
        // szToken will contain name and pszRemainingTxt will point to the first
		// of two integer values: depart time and next arrival time
		pszRemainingTxt = getToken(szInputBuffer, szToken, sizeof(Token)-1);
			
        iScanfCnt = sscanf(pszRemainingTxt, "%d %d[^\n]\n"   
                , &iDepartureUnits                    // time when person will depart
                , &iNextArrivalTime);                 // time when next person will arrive
		// check for bad input resulting in conversion issues
        if (iScanfCnt < 2)
           ErrExit(ERR_ALGORITHM, "Input conversion failed");
		
		addEventNodes(list, szToken, iDepartureUnits, iCurrentArriveTime);
		// update iArriveTime to reflect when next person will arrive
		iCurrentArriveTime += iNextArrivalTime;
	} // end while
	return iCurrentArriveTime;
}
/******************** printLL *********************************************
void printLL(LinkedList list)
Purpose:
    Takes a linked list and prints it out iteratively.
Parameters:
    I  LinkedList list       pointer to a LinkedListImp

Returns:
    N/A
Notes:
    N/A
**************************************************************************/
void printLL(LinkedList list)
{
	NodeLL *p;
	
	printf("%-5s %-20s %-15s\n", "Time", "Person", "Event");
	printf("%-45s\n", "-----------------------------------");
	// iterate node-by-node printing each event
	for (p = list->pHead; p != NULL; p = p->pNext)
	{
		// print list node-by-node
		printf("%-5d %-20s %-15s\n"
			, p->event.iTime
			, p->event.person.szName
			, (p->event.iEventType == EVT_ARRIVE) ? ("Arrive") : ("Depart"));
	}
}
/******************** printEventNode **************************************
void printEventNode(LinkedList list)
Purpose:
    Takes an event structure and prints it out
Parameters:
    I  LinkedList list       pointer to a LinkedListImp

Returns:
    N/A
Notes:
    N/A
**************************************************************************/
void printEventNode(Event printEvent)
{
	static int i = 0;
	
	if (printEvent.iTime == 0 && i == 0)
	{
		printf("%-5s %-20s %-15s\n", "Time", "Person", "Event");
		printf("%-45s\n", "-----------------------------------");
	}
	
	printf("%-5d %-20s %-15s\n"
			, printEvent.iTime
			, printEvent.person.szName
			, (printEvent.iEventType == EVT_ARRIVE) ? ("Arrive") : ("Depart"));
	
	i++;
}
/************************** freeLL **************************************
void freeLL(LinkedList list)
Purpose:
    Correctly frees an entire linked list ensuring no memory leaks.
Parameters:
    I  LinkedList list       pointer to a LinkedListImp

Returns:
    N/A
Notes:
    First, this function frees the NodeLL's which have been allocated 
    memory.  Next, the actual memory allocated for the LinkedListImp
    structure is freed.  Finally, the pointer pointing to the
    LinkedListImp is set to NULL.
**************************************************************************/
void freeLL(LinkedList list)
{
	NodeLL *pRemove;           // used to iterate through
	                           // LL 
	Event removeEvent;   // used to store returned 
	                     // event from removeLL
	// list is either free or dangling
	// not much we can do
	if (list->pHead == NULL)
	{
		free(list);
		return;
	}
	// iterate node-by-node freeing each node
	for (pRemove = list->pHead; pRemove != NULL; pRemove = pRemove->pNext)
	{
		removeLL(list, &removeEvent);     // function frees node at i
	}
	// finally free the memory allocated for LinkedListImp structure
	free(list);
	// just to make sure the pointer is not used improperly in the future
	list->pHead = NULL;
}
/************************** newSim *******************************************************
Simulation newSim(LinkedList list)
Purpose:
    Conducts a simulation using the linked list with event nodes.
Parameters:
    I  Simulation simulation     pointer to a SimulationImp structure
    I  int iTimeLimit            maximum time allowed before function return
Returns:
    A pointer to a SimulationImp structure
Notes:
    If memory is unable to be allocated the program aborts.
*******************************************************************************************/
Simulation newSim(LinkedList list)
{
	Simulation simulation = (Simulation) malloc(sizeof(SimulationImp));
	
	if (simulation == NULL)
	    ErrExit(ERR_ALGORITHM, "No memory available for simulation");
	// set simulation to point to first node in list
	simulation->eventList = list;
	
	return simulation;
}
/******************** runSimulation *******************************************************
void runSimulation(Simulation simulation, int iTimeLimit)
Purpose:
    Conducts a simulation using the linked list with event nodes.
Parameters:
    I  Simulation simulation     pointer to a SimulationImp structure
    I  int iTimeLimit            maximum time allowed before function return
Returns:
    N/A
Notes:
    The simulation works by iteratively removing the first node in the linked list.
    The event returned from removeLL (via param) is used for the switch to determine
    which action to take.  If returned event value from removeLL is not recognized,
    the program is aborted.  
*******************************************************************************************/
void runSimulation(Simulation simulation, int iTimeLimit)
{
	Event removedEvent;           // stores NodeLL returned from removeLL	
	
	while(removeLL(simulation->eventList, &removedEvent))
	{
		// prevent infinite loop
		if (simulation->iClock > iTimeLimit)
			break;
		
		simulation->iClock = removedEvent.iTime;
		switch(removedEvent.iEventType)
		{
			case EVT_ARRIVE:
			    printEventNode(removedEvent);
			    break;
			case EVT_DEPART:
			    printEventNode(removedEvent);
			    break;
			default:
			    ErrExit(ERR_ALGORITHM, "Attempt to run simulation with unknown event");
		} // end switch
	} // end while
	printf("%-5d %-20s %-15s\n"
		, simulation->iClock
		, "SIMULATION"
		, "TERMINATES");
}

int main(void)
{
	LinkedList list = newLinkedList();          // LinkedList
	Simulation sim;                             // simulation
	
	// takes a line of text from stdin and stores it in LinkedList
	readEventData(list);
	
	// sim must be allocated here so function can point to 
	// list properly
	sim = newSim(list);         
	runSimulation(sim, 1000);

	// ensure memory is not leaked
	freeLL(list);
	free(sim);
	
	return (EXIT_SUCCESS);
}

// functions in most programs, but require modifications
/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    In general, this routine optionally prints error messages and diagnostics.
    It also prints usage information.

    If this is an argument error (iArg >= 0), it prints a formatted message 
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits 
    with ERR_COMMAND_LINE.

    If this is a usage error (but not specific to the argument), it prints 
    the specific message and its supplemental diagnostic information.  It 
    also shows the usage and exist with ERR_COMMAND_LINE. 

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript or control:
                                    > 0 - command argument subscript
                                    0 - USAGE_ONLY - show usage only
                                    -1 - USAGE_ERR - show message and usage
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    switch (iArg)
    {
        case USAGE_ERR:
            fprintf(stderr, "Error: %s %s\n"
                , pszMessage
                , pszDiagnosticInfo);
            break;
        case USAGE_ONLY:
            break;
        default:
            fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
                , iArg
                , pszMessage
                , pszDiagnosticInfo);
    }
    // print the usage information for any type of command line error
    fprintf(stderr, "p2 -c customerFileName -q queryFileName\n");
    if (iArg == USAGE_ONLY)
        exit(USAGE_ONLY); 
    else 
        exit(ERR_COMMAND_LINE);
}

// Utility routines provided by Larry (copy from program #2)
/******************** ErrExit **************************************
  void ErrExit(int iexitRC, char szFmt[], ... )
Purpose:
    Prints an error message defined by the printf-like szFmt and the
    corresponding arguments to that function.  The number of 
    arguments after szFmt varies dependent on the format codes in
    szFmt.  
    It also exits the program with the specified exit return code.
Parameters:
    I   int iexitRC             Exit return code for the program
    I   char szFmt[]            This contains the message to be printed
                                and format codes (just like printf) for 
                                values that we want to print.
    I   ...                     A variable-number of additional arguments
                                which correspond to what is needed
                                by the format codes in szFmt. 
Notes:
    - Prints "ERROR: " followed by the formatted error message specified 
      in szFmt. 
    - Prints the file path and file name of the program having the error.
      This is the file that contains this routine.
    - Requires including <stdarg.h>
Returns:
    Returns a program exit return code:  the value of iexitRC.
**************************************************************************/
void ErrExit(int iexitRC, char szFmt[], ... )
{
    va_list args;               // This is the standard C variable argument list type
    va_start(args, szFmt);      // This tells the compiler where the variable arguments
                                // begins.  They begin after szFmt.
    printf("ERROR: ");
    vprintf(szFmt, args);       // vprintf receives a printf format string and  a
                                // va_list argument
    va_end(args);               // let the C environment know we are finished with the
                                // va_list argument
    printf("\n");
    exit(iexitRC);
}

/******************** getToken **************************************
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
Purpose:
    Examines the input text to return the next token.  It also
    returns the position in the text after that token.  This function
    does not skip over white space, but it assumes the input uses 
    spaces to separate tokens.
Parameters:
    I   char *pszInputTxt       input buffer to be parsed
    O   char szToken[]          Returned token.
    I   int iTokenSize          The size of the token variable.  This is used
                                to prevent overwriting memory.  The size
                                should be the memory size minus 1 (for
                                the zero byte).
Returns:
    Functionally:
        Pointer to the next character following the delimiter after the token.
        NULL - no token found.
    szToken parm - the returned token.  If not found, it will be an
        empty string.
Notes:
    - If the token is larger than the szToken parm, we return a truncated value.
    - If a token isn't found, szToken is set to an empty string
    - This function does not skip over white space occurring prior to the token.
**************************************************************************/
char * getToken(char *pszInputTxt, char szToken[], int iTokenSize)
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    char szDelims[20] = " \n\r";        // delimiters
    szToken[0] = '\0';

    // check for NULL pointer 
    if (pszInputTxt == NULL)
        ErrExit(ERR_ALGORITHM
        , "getToken passed a NULL pointer");

    // Check for no token if at zero byte
    if (*pszInputTxt == '\0')
        return NULL;

    // get the position of the first delim
    iDelimPos = strcspn(pszInputTxt, szDelims);

    // if the delim position is at the first character, return no token.
    if (iDelimPos == 0)
        return NULL;

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > iTokenSize)
        iCopy = iTokenSize;             // truncated size
    else
        iCopy = iDelimPos;

    // copy the token into the target token variable
    memcpy(szToken, pszInputTxt, iCopy);
    szToken[iCopy] = '\0';              // null terminate

    // advance the position
    pszInputTxt += iDelimPos;
    if (*pszInputTxt == '\0')
        return pszInputTxt;
    else
        return pszInputTxt + 1;
}