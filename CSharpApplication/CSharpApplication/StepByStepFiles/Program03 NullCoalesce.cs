using System;

class NullCoalesce
{
    class KPerson
    {
        public string FirstName;
    }

    static int? GetNullableInt()
    {
        return null;
    }

    static string GetStringValue()
    {
        return null;
    }

    static void Main()
    {
        Nullable<int> i = null;
        if( i.HasValue == true )
            Console.WriteLine( i.Value );
        i = 9;
        if( i.HasValue == true )
            Console.WriteLine( "i=" + i.Value.ToString() );

        // nullable int type
        int? j = null;
        if( j != null )
            Console.WriteLine( j );
        j = 99;
        if( j != null )
            Console.WriteLine( "j=" + j.ToString() );

        int? x = null;

        // null coalesce operator
        // Set y to the value of x if x is NOT null; otherwise,
        // if x == null, set y to -1.
        int y = x ?? -1;

        // Assign i to return value of the method if the method's result
        // is NOT null; otherwise, if the result is null, set i to the
        // default value of int.
        int k = GetNullableInt() ?? default( int );

        string s = GetStringValue();
        // Display the value of s if s is NOT null; otherwise, 
        // display the string "Unspecified".
        Console.WriteLine( s ?? "Unspecified" );

        // conditional null operator
        KPerson person = null;
        var first = person?.FirstName ?? "(unknown)";
        Console.WriteLine( first );
    }
}
