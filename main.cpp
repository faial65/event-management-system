//Inheritance
//friend function (A friend function is a function that is not a member of a class but is given special access to the private and protected members of that class. In the code, a friend function is used in the Event class to overload the << (output stream) operator for displaying event details.)
//operator overloading (operator== and operator!= are overloaded to compare two UserBase objects based on their username and password attributes. operator== and operator!= are overloaded to compare two UserBase objects based on their username and password attributes.)
//file
//polymorphism
//abstract classes (EventTicket defines a common interface for all event tickets by declaring pure virtual functions (displayTicketInfo and getTicketPrice).)
/*templates (The UserBase class is a template class that serves as a base class for user-related operations. It is templated on the type T, which is used to represent the derived User class. This allows you to create a user class with specific data types for the username and password.EventBase Template Class:
Similarly, the EventBase class is a template class used as a base class for event-related operations. It is templated on the type T, which represents the derived Event class. This allows you to create event classes with specific data types for event details.)*/
//stl
//encapsulation
//smart pointer std::shared_ptr  to manage the ownership and lifetime of EventTicket objects stored in vectors.Smart pointers ensure that memory is properly managed, and objects are automatically deallocated when they are no longer needed.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>  // For smart pointers
using namespace std;

// Base template class for User
template <typename T>
class UserBase
{
protected:
    string username;
    string password;

public:
    UserBase(string uname, string pwd) : username(uname), password(pwd) {}

    bool operator==(const T& other) const
    {
        return (username == other.username && password == other.password);
    }

    bool operator!=(const T& other) const
    {
        return !(*this == other);
    }
};

// Template class for User
class User : public UserBase<User>
{
public:
    using UserBase<User>::UserBase;

    static bool registerUser(const string& username, const string& password)
    {
        ifstream userFile("users.txt");
        if (userFile.is_open())
        {
            string line;
            while (getline(userFile, line))
            {
                size_t pos = line.find('|');
                string existingUsername = line.substr(0, pos);
                if (existingUsername == username)
                {
                    userFile.close();
                    return false; // if Username already exists
                }
            }
            userFile.close();
        }

        ofstream outFile("users.txt", ios::app);
        if (outFile.is_open())
        {
            outFile << username << "|" << password << "\n";
            outFile.close();
            return true; //the  Registration is successful
        }
        else
        {
            cout << "Failed to open user database.\n";
            return false;
        }
    }

    static bool loginUser(const string& username, const string& password)
    {
        ifstream userFile("users.txt");
        if (userFile.is_open())
        {
            string line;
            while (getline(userFile, line))
            {
                size_t pos = line.find('|');
                string existingUsername = line.substr(0, pos);
                string existingPassword = line.substr(pos + 1);
                if (existingUsername == username && existingPassword == password)
                {
                    userFile.close();
                    return true; // Login successful
                }
            }
            userFile.close();
        }
        return false; // if Login failed
    }
};

// Abstract base class for Event Tickets
class EventTicket
{
protected:
    string ticketType;
    double ticketPrice;

public:
    EventTicket(string type, double price) : ticketType(type), ticketPrice(price) {}

    virtual void displayTicketInfo() const = 0; // Pure virtual function

    virtual double getTicketPrice() const
    {
        return ticketPrice;
    }

    virtual ~EventTicket() {}
};

// Derived class for Regular Tickets
class RegularTicket : public EventTicket
{
public:
    RegularTicket(double price) : EventTicket("Regular", price) {}

    void displayTicketInfo() const override
    {
        cout << "Ticket Type: " << ticketType << endl;
        cout << "Price: $" << ticketPrice << endl;
        cout << "This is a regular ticket.\n";
    }
};

// Derived class for VIP Tickets
class VIPTicket : public EventTicket
{
public:
    VIPTicket(double price) : EventTicket("VIP", price) {}

    void displayTicketInfo() const override
    {
        cout << "Ticket Type: " << ticketType << endl;
        cout << "Price: $" << ticketPrice << endl;
        cout << "This is a VIP ticket.\n";
    }
};

// Base template class for Event
template <typename T>
class EventBase
{
protected:
    string eventName;
    string eventDate;
    string eventLocation;
    int maxAttendees;

public:
    EventBase(string name, string date, string location, int max) :
        eventName(name), eventDate(date), eventLocation(location), maxAttendees(max) {}

    bool operator==(const T& other) const
    {
        return (eventName == other.eventName && eventDate == other.eventDate);
    }

    bool operator!=(const T& other) const
    {
        return !(*this == other);
    }
};

// Template class for Event
class Event : public EventBase<Event>
{
private:
    int ticketsSold;
    vector<shared_ptr<EventTicket>> tickets;  // Vector to store event tickets

    // Friend function to display event details
    friend ostream& operator<<(ostream& os, const Event& event);

public:
    void displayAvailableTickets() const
    {
        cout << "Available Tickets:\n";
        for (size_t i = 0; i < tickets.size(); ++i)
        {
            cout << i + 1 << ". ";
            tickets[i]->displayTicketInfo();
        }
    }

    vector<shared_ptr<EventTicket>> getEventTickets() const
    {
        return tickets;
    }

    string getEventName() const
    {
        return eventName;
    }

    string getEventDate() const
    {
        return eventDate;
    }

    string getEventLocation() const
    {
        return eventLocation;
    }

    int getMaxAttendees() const
    {
        return maxAttendees;
    }

    using EventBase<Event>::EventBase;

    Event(string name, string date, string location, int max) :
        EventBase<Event>(name, date, location, max), ticketsSold(0) {}

    void displayEventInfo() const
    {
        cout << *this;  // Use  the   overloaded operator to display event details
    }

    // Adding a regular ticket to the event
    void addRegularTicket(double price)
    {
        tickets.push_back(make_shared<RegularTicket>(price));
    }

    // Adding a VIP ticket to the event
    void addVIPTicket(double price)
    {
        tickets.push_back(make_shared<VIPTicket>(price));
    }

    void registerAttendee()
    {
        if (ticketsSold < maxAttendees)
        {
            cout << "Enter attendee's name: ";
            string attendeeName;
            cin.ignore();
            getline(cin, attendeeName);
            cout << "Attendee " << attendeeName << " registered for the event.\n";
            ++ticketsSold;
        }
        else
        {
            cout << "Event is full. No more attendees can be registered.\n";
        }
    }

    // Selling a ticket in counter
    void sellTicket()
    {
        if (ticketsSold < maxAttendees)
        {
            displayAvailableTickets(); // Show available tickets

            cout << "Enter the ticket number to sell (1-" << tickets.size() << "): ";
            int ticketNumber;
            cin >> ticketNumber;

            if (ticketNumber >= 1 && ticketNumber <= static_cast<int>(tickets.size()))
            {
                double price = tickets[ticketNumber - 1]->getTicketPrice();
                cout << "Ticket sold for $" << price << ".\n";

                // Remove the sold ticket
                tickets.erase(tickets.begin() + ticketNumber - 1);

                ++ticketsSold;
            }
            else
            {
                cout << "Invalid ticket number.\n";
            }
        }
        else
        {
            cout << "Event is full. No more tickets can be sold.\n";
        }
    }

    int getAvailableTickets() const
    {
        return maxAttendees - ticketsSold;
    }

    // Overloaded operator to display event details
    friend ostream& operator<<(ostream& os, const Event& event)
    {
        os << "Event Name: " << event.eventName << endl;
        os << "Date: " << event.eventDate << endl;
        os << "Location: " << event.eventLocation << endl;
        os << "Max Attendees: " << event.maxAttendees << endl;
        os << "Tickets Sold: " << event.ticketsSold << endl;
        os << "Available Tickets: " << event.getAvailableTickets() << endl;
        os << "Event Tickets:\n";
        for (size_t i = 0; i < event.tickets.size(); ++i)
        {
            os << i + 1 << ". ";
            event.tickets[i]->displayTicketInfo();
        }
        os << "---------------------------\n";
        return os;
    }
};

// Template class for EventManager
template <typename EventT>
class EventManager
{
private:
    vector<EventT> events;

public:
    vector<EventT>& getEvents()
    {
        return events;
    }

    void loadEventsFromFile()
    {
        ifstream inFile("events.txt");
        if (!inFile.is_open())
        {
            cout << "Failed to open events database file.\n";
            return;
        }

        events.clear(); // Clear existing events

        string line;
        while (getline(inFile, line))
        {
            size_t pos = line.find('|');
            string name = line.substr(0, pos);
            string date = line.substr(pos + 1);

            // Read the rest of the event details
            string location;
            int maxAttendees;
            if (getline(inFile, location) && getline(inFile, line))
            {
                maxAttendees = stoi(line);

                // Create an event and add it to the vector
                EventT event(name, date, location, maxAttendees);
                events.push_back(event);

                // Read and add tickets to the event
                while (getline(inFile, line))
                {
                    if (line == "Regular")
                    {
                        double price;
                        if (getline(inFile, line))
                        {
                            price = stod(line);
                            event.addRegularTicket(price);
                        }
                    }
                    else if (line == "VIP")
                    {
                        double price;
                        if (getline(inFile, line))
                        {
                            price = stod(line);
                            event.addVIPTicket(price);
                        }
                    }
                    else if (line == "EndTickets")
                    {
                        break;
                    }
                }
            }
        }

        inFile.close();
    }

    void saveEventsToFile()
    {
        ofstream outFile("events.txt");
        if (!outFile.is_open())
        {
            cout << "Failed to open events database file for writing.\n";
            return;
        }

        for (const EventT& event : events)
        {
            outFile << event.getEventName() << "|" << event.getEventDate() << "\n";
            outFile << event.getEventLocation() << "\n";
            outFile << event.getMaxAttendees() << "\n";

            // Get event tickets using the  function
            vector<shared_ptr<EventTicket>> eventTickets = event.getEventTickets();

            // Write event tickets
            for (const shared_ptr<EventTicket>& ticket : eventTickets)
            {
                if (RegularTicket* regTicket = dynamic_cast<RegularTicket*>(ticket.get()))
                {
                    outFile << "Regular" << "\n";
                    outFile << regTicket->getTicketPrice() << "\n";
                }
                else if (VIPTicket* vipTicket = dynamic_cast<VIPTicket*>(ticket.get()))
                {
                    outFile << "VIP" << "\n";
                    outFile << vipTicket->getTicketPrice() << "\n";
                }
            }

            outFile << "EndTickets" << "\n";
        }

        outFile.close();
    }

    void createEvent(const string& name, const string& date, const string& location, int max)
    {
        EventT event(name, date, location, max);
        events.push_back(event);
        cout << "Event created successfully.\n";
    }

    void modifyEvent(const string& name, const string& date)
    {
        for (EventT& event : events)
        {
            if (event == EventT(name, date, "", 0))
            {
                string newName, newDate, newLocation;
                int newMax;
                cout << "Enter new Event Name: ";
                cin.ignore();
                getline(cin, newName);
                cout << "Enter new Event Date: ";
                getline(cin, newDate);
                cout << "Enter new Event Location: ";
                getline(cin, newLocation);
                cout << "Enter new Max Attendees: ";
                cin >> newMax;

                event = EventT(newName, newDate, newLocation, newMax);
                cout << "Event modified successfully.\n";
                return;
            }
        }
        cout << "Event not found.\n";
    }

    void deleteEvent(const string& name, const string& date)
    {
        for (auto it = events.begin(); it != events.end(); ++it)
        {
            if (*it == EventT(name, date, "", 0))
            {
                events.erase(it);
                cout << "Event deleted successfully.\n";
                return;
            }
        }
        cout << "Event not found.\n";
    }

    void displayAllEvents()
    {
        for (const EventT& event : events)
        {
            event.displayEventInfo();
        }
    }
};

int main()
{
    EventManager<Event> eventManager;
    eventManager.loadEventsFromFile(); // Load events from file

    int choice;
    bool loggedIn = false;
    string username;

    do
    {
        cout << "Event Management System\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Create Event\n";
        cout << "4. Modify Event\n";
        cout << "5. Delete Event\n";
        cout << "6. Display All Events\n";
        cout << "7. Logout\n";
        cout << "8. Register Attendee\n";
        cout << "9. Sell Ticket\n";
        cout << "10. Check Available Tickets\n";
        cout << "11. Exit\n";

        if (loggedIn)
        {
            cout << "Logged in as: " << username << endl;
        }

        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice)
        {
        case 1:
        {
            string uname, pwd;
            cout << "Enter username: ";
            cin >> uname;
            cout << "Enter password: ";
            cin >> pwd;

            cin.ignore(); // Clear the newline character

            if (User::registerUser(uname, pwd))
            {
                cout << "Registration successful.\n";
            }
            else
            {
                cout << "Registration failed. Username already exists.\n";
            }
            break;
        }
        case 2:
        {
            string uname, pwd;
            cout << "Enter username: ";
            cin >> uname;
            cout << "Enter password: ";
            cin >> pwd;
            cin.ignore(); // Clear the newline character
            if (User::loginUser(uname, pwd))
            {
                cout << "Login successful.\n";
                loggedIn = true;
                username = uname;
            }
            else
            {
                cout << "Login failed. Invalid username or password.\n";
            }
            break;
        }
        case 3: // Create Event
        {
            if (loggedIn)
            {
                string name, date, location;
                int max;
                cout << "Enter Event Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Event Date: ";
                getline(cin, date);
                cout << "Enter Event Location: ";
                getline(cin, location);
                cout << "Enter Max Attendees: ";
                cin >> max;

                eventManager.createEvent(name, date, location, max);

                // Now, you can add tickets to the newly created event.
                Event& newEvent = eventManager.getEvents().back(); // Get the newly created event

                // Add Regular Tickets
                int numRegularTickets;
                cout << "Enter the number of Regular Tickets to add: ";
                cin >> numRegularTickets;
                for (int i = 0; i < numRegularTickets; ++i)
                {
                    double regularTicketPrice;
                    cout << "Enter Regular Ticket #" << i + 1 << " price: $";
                    cin >> regularTicketPrice;
                    newEvent.addRegularTicket(regularTicketPrice);
                }

                // Add VIP Tickets
                int numVIPTickets;
                cout << "Enter the number of VIP Tickets to add: ";
                cin >> numVIPTickets;
                for (int i = 0; i < numVIPTickets; ++i)
                {
                    double vipTicketPrice;
                    cout << "Enter VIP Ticket #" << i + 1 << " price: $";
                    cin >> vipTicketPrice;
                    newEvent.addVIPTicket(vipTicketPrice);
                }

                cout << "Event created successfully.\n";
                eventManager.saveEventsToFile(); // Save events to file after adding a new event
            }
            else
            {
                cout << "You need to log in to create an event.\n";
            }
            break;
        }
        case 4:
        {
            if (loggedIn)
            {
                string name, date;
                cout << "Enter Event Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Event Date: ";
                getline(cin, date);

                eventManager.modifyEvent(name, date);
                cout << "Event modified successfully.\n";
                eventManager.saveEventsToFile(); // Save events to file after modifying an event
            }
            else
            {
                cout << "You need to log in to modify an event.\n";
            }
            break;
        }
        case 5:
        {
            if (loggedIn)
            {
                string name, date;
                cout << "Enter Event Name to Delete: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Event Date to Delete: ";
                getline(cin, date);

                eventManager.deleteEvent(name, date);
                cout << "Event deleted successfully.\n";
                eventManager.saveEventsToFile(); // Save events to file after deleting an event
            }
            else
            {
                cout << "You need to log in to delete an event.\n";
            }
            break;
        }
        case 6:
        {
            if (loggedIn)
            {
                eventManager.displayAllEvents();
            }
            else
            {
                cout << "You need to log in to see the events.\n";
            }
            break;
        }
        case 7:
        {
            loggedIn = false;
            username = "";
            cout << "Logged out.\n";
            break;
        }
        case 8:   // Register Attendee
        {
            if (loggedIn)
            {
                string name, date;
                cout << "Enter Event Name: ";
                getline(cin, name);
                cout << "Enter Event Date: ";
                getline(cin, date);

                // Find the event
                for (Event& event : eventManager.getEvents())
                {
                    if (event == Event(name, date, "", 0))
                    {
                        event.registerAttendee();
                        eventManager.saveEventsToFile(); // Save events to file after registering an attendee
                        break;
                    }
                }
            }
            else
            {
                cout << "You need to log in to register an attendee.\n";
            }
            break;
        }
        case 9:   // Sell Ticket
        {
            if (loggedIn)
            {
                string name, date;
                cout << "Enter Event Name: ";
                getline(cin, name);
                cout << "Enter Event Date: ";
                getline(cin, date);

                // Find the event
                for (Event& event : eventManager.getEvents())
                {
                    if (event == Event(name, date, "", 0))
                    {
                        event.sellTicket();
                        eventManager.saveEventsToFile(); // Save events to file after selling a ticket
                        break;
                    }
                }
            }
            else
            {
                cout << "You need to log in to sell a ticket.\n";
            }
            break;
        }
        case 10:   // Check Available Tickets
        {
            if (loggedIn)
            {
                cout << "Available Events:\n";
                for (const Event& event : eventManager.getEvents())
                {
                    cout << "Event Name: " << event.getEventName() << endl;
                    cout << "Event Date: " << event.getEventDate() << endl;
                }

                string name, date;
                cout << "Enter Event Name: ";
                getline(cin, name);
                cout << "Enter Event Date: ";
                getline(cin, date);

                // Find the event
                for (const Event& event : eventManager.getEvents())
                {
                    if (event == Event(name, date, "", 0))
                    {
                        cout << "Available Tickets: " << event.getAvailableTickets() << endl;
                        break;
                    }
                }
            }
            else
            {
                cout << "You need to log in to check available tickets.\n";
            }
            break;
        }
        case 11:
        {
            cout << "Exiting...\n";
            break;
        }
        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }

        cout << "Press Enter to continue...";
        cin.get();

    } while (choice != 11);

    return 0;
}
