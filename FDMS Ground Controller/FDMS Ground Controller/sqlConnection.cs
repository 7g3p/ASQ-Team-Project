using Microsoft.VisualBasic;
using MySqlConnector;
using System;
using System.Collections.Generic;
using System.Data;
using System.Text;
using System.Windows.Forms;


namespace FDMS_Ground_Controller
{
    /*
    * NAME : TCPConnection
    * PURPOSE : This class creates, instatiates, and connects to a SQL database and manipulates telemetry data in said database
    */
    public class sqlConnection
    {
        private MySqlConnection connection;
        private bool isOpen = false;

        /*
        * FUNCTION : Initialize()
        * DESCRIPTION :
        *	This function initializes the sql connection to the database
        * PARAMETERS :
        *	N/A
        * RETURNS :
        *	N/A
        */
        public void Initialize()
        {
            string connectionString = "SERVER=gator4235.hostgator.com; DATABASE=webus137_FDMS; UID=webus137_fdms1; PASSWORD=Admin;";

            connection = new MySqlConnection(connectionString);
        }

        /*
        * FUNCTION : startConnection()
        * DESCRIPTION :
        *	This function starts the connection with the sql database. Notifies the user if connection could not be made.
        * PARAMETERS :
        *	N/A
        * RETURNS :
        *	    bool : returns the success(true) or failure(false) of the function
        */
        private bool startConnection()
        {
            try
            {
                connection.Open();
                isOpen = true;
                return true;
            }
            catch(Exception e) {
                MessageBox.Show(e.Message);
                return false;
            }
        }

        /*
        * FUNCTION : stopConnection()
        * DESCRIPTION :
        *	This function ends the connection with the sql database. 
        * PARAMETERS :
        *	N/A
        * RETURNS :
        *	    bool : returns the success(true) or failure(false) of the function
        */
        private bool stopConnection()
        {
            try
            {
                connection.Close();
                return true;
            }
            catch
            {
                return false;
            }
        }

        /*
        * FUNCTION : insert(string uparsedData)
        * DESCRIPTION :
        *	This function takes the received telemtry data, parses it, and inserts the tokens into the appropriate sections of the database
        * PARAMETERS :
        *	    string unparsedData : The received telemetry data prior to parsing
        * RETURNS :
        *	 N/A
        */
        public void insert(string unparsedData)
        {
            //parse data here
            StringBuilder sb = new StringBuilder("INSERT INTO flightInfo (tailNumber, timeStamp, accelx, accely, accelz, weight, altitude, pitch, bank) VALUES (");

            //Deconstruct string
            string[] delimited = unparsedData.Split(',');

            sb.Append("'"+delimited[0] + "','" + delimited[2] + "'," + delimited[3] + "," + delimited[4] + "," + 
                delimited[5] + "," + delimited[6] + "," + delimited[7] + "," + delimited[8] + "," +delimited[9]+");");

            //check for checksum
            float alt = float.Parse(delimited[7]);
            float pitch = float.Parse(delimited[8]);
            float bank = float.Parse(delimited[9]);
            int checksum = (int)(alt + pitch + bank) / 3;
            
            // Verify the checksum should be what it is
            if (checksum == int.Parse(delimited[10]))
            {
                if (this.startConnection() == true)
                {
                    try
                    {
                        MySqlCommand com = new MySqlCommand(sb.ToString(), connection);

                        com.ExecuteNonQuery();

                        this.stopConnection();
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show(e.Message);
                    }
                }

                else
                {
                    MySqlCommand com = new MySqlCommand(sb.ToString(), connection);

                    com.ExecuteNonQuery();
                }
            }
        }

        /*
        * FUNCTION : select(string tailNum)
        * DESCRIPTION :
        *	This function returns the data set of telemetry data, from the database, of the chosen aircraft based on the aircraftTailNumber.
        * PARAMETERS :
        *	    string tailNum : The chosen aircraft's tail number
        * RETURNS :
        *	    DataSet : The data set of telemetry for the chosen aircraft
        */
        public DataSet select(string tailNum)
        {
            // Check if the connection is there
            if (this.startConnection() == true)
            {
                try
                {
                    // Selects the data set from the database
                    StringBuilder sb = new StringBuilder(@"SELECT * FROM flightInfo WHERE tailNumber='");
                    sb.Append(tailNum + "';");

                    MySqlCommand com = new MySqlCommand(sb.ToString(), connection);

                    MySqlDataAdapter adapter = new MySqlDataAdapter(com);

                    DataSet ds = new DataSet();

                    adapter.Fill(ds);

                    this.stopConnection();
                    return ds;
                }
                catch(Exception e)
                {
                    
                }
            }
            DataSet err = new DataSet();
            return err;
        }
    }
}
