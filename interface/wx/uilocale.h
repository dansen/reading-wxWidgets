///////////////////////////////////////////////////////////////////////////////
// Name:        wx/uilocale.h
// Purpose:     Interface of wxUILocale
// Author:      Vadim Zeitlin
// Created:     2021-08-01
// Copyright:   (c) 2021 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
    Query and modify locale used for the UI by the current platform.

    UI locale determines all culture-dependent conventions used in the user
    interface, including numbers, currencies and dates formatting. It also
    determines the language used by the native dialogs, such as wxFileDialog,
    where different labels use the language corresponding to the current UI
    locale.

    The UI locale is, in general, different from C locale set by the standard
    @c setlocale() function and affecting C standard library functions such as
    @c printf(), @c scanf(), @c strftime() and many others. Unfortunately, the
    relationship between C and UI locales is not the same depending on the
    platform: with wxGTK they must be the same, but under macOS C locale must
    not be changed, as doing this exposes bugs in the system. Because of this,
    applications can't generally count on C locale being set to any particular
    value and it is best to avoid using it, including implicitly via the
    standard C functions, in portable code. Instead, consider using
    wxNumberFormatter for parsing and formatting numbers according to the
    current UI locale or wxString::FromCDouble() and wxString::ToCDouble()
    functions for doing it always using period as decimal separator.

    Localized applications should call wxUILocale::UseDefault() on startup to
    explicitly indicate that they opt-in using the current UI locale, even if
    this results in changing the global C locale, as is the case in wxGTK. Note
    that some platforms (MSW and macOS) will use default user locale for their
    standard dialogs even if this function is not called, but it is still
    necessary to call it to use the correct number and date formats and to
    avoid mixing messages in the user language with default formats not
    corresponding to it.

    Please also note that under macOS to really use the user locale, it must be
    listed as a supported language in the application @c Info.plist file under
    @c CFBundleLocalizations key.

    Unlike wxLocale class, this class doesn't affect the translations used by
    the application, see wxTranslations for doing this.

    @library{wxbase}
    @since 3.1.6
 */
class wxUILocale
{
public:
    /**
        Configure the UI to use the default user locale.

        Localized applications should call this functions as early as possible
        during the program startup, e.g. in the very beginning of the
        overridden wxApp::OnInit().

        Note that under most Unix systems (but not macOS) this function changes
        the C locale to the locale specified by the environment variables and
        so affects the results of calling C functions such as @c sprintf() etc
        which can use comma, rather than period, as decimal separator. The
        wxString::ToCDouble() and wxString::FromCDouble() functions can be used
        for parsing and formatting floating point numbers using period as
        decimal separator independently of the current locale.

        @return @true on success or @false if the default locale couldn't be set
     */
    static bool UseDefault();

    /**
        Get the object corresponding to the currently used locale.

        If UseDefault() had been called, this object corresponds to the default
        user locale. Otherwise it corresponds to a generic locale similar to
        "C" locale, i.e. always uses period as decimal separator and m/d/y date
        format.
     */
    static const wxUILocale& GetCurrent();

    /**
        Get the platform-dependent name of the current locale.

        This name can be used in diagnostic messages.
     */
    wxString GetName() const;

    /**
        Query the locale for the specified information.

        This function returns the value of the locale-specific option specified
        by the given @a index.

        @param index
            One of the elements of wxLocaleInfo enum.
        @param cat
            The category to use with the given index or wxLOCALE_CAT_DEFAULT if
            the index can only apply to a single category.
        @return
            The option value or empty string if the function failed.
     */
    wxString GetInfo(wxLocaleInfo index,
                     wxLocaleCategory cat = wxLOCALE_CAT_DEFAULT) const;
};

/**
    Return the format to use for formatting user-visible dates.

    This is a simple wrapper function normally calling wxUILocale::GetInfo()
    with wxLOCALE_SHORT_DATE_FMT argument, but which is also available when @c
    wxUSE_INTL==0, i.e. support for internationalization is disabled at
    compile-time, in which case it returns @c %x string, i.e. uses the current
    C locale formatting rather than UI locale.

    @see wxDateTime::Format()

    @since 3.1.6
 */
wxString wxGetUIDateFormat();