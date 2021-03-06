/*
    Copyright 2007-2008 by Robert Knight <robertknight@gmail.com>
    Copyright 2018 by Harald Sitter <sitter@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#ifndef EDITPROFILEDIALOG_H
#define EDITPROFILEDIALOG_H

// Qt
#include <QAbstractItemDelegate>
#include <QDialog>
#include <QHash>
#include <QPointer>

// KDE
#include <KNS3/Entry>

// Konsole
#include "Profile.h"
#include "Enumeration.h"
#include "ColorScheme.h"
#include "ColorSchemeEditor.h"
#include "KeyboardTranslatorManager.h"

class QAbstractButton;
class QItemSelectionModel;
class QTextCodec;
class QDialogButtonBox;
namespace Ui {
class EditProfileDialog;
}

namespace Konsole {
/**
 * A dialog which allows the user to edit a profile.
 * After the dialog is created, it can be initialized with the settings
 * for a profile using setProfile().  When the user makes changes to the
 * dialog and accepts the changes, the dialog will update the
 * profile in the SessionManager by calling the SessionManager's
 * changeProfile() method.
 *
 * Some changes made in the dialog are preview-only changes which cause
 * the SessionManager's changeProfile() method to be called with
 * the persistent argument set to false.  These changes are then
 * un-done when the dialog is closed.
 */
class KONSOLEPRIVATE_EXPORT EditProfileDialog : public QDialog
{
    Q_OBJECT

public:
    /** Constructs a new dialog with the specified parent. */
    explicit EditProfileDialog(QWidget *parent = nullptr);
    ~EditProfileDialog() Q_DECL_OVERRIDE;

    /**
     * Initializes the dialog with the settings for the specified session
     * type.
     *
     * When the dialog closes, the profile will be updated in the SessionManager
     * with the altered settings.
     *
     * @param profile The profile to be edited
     */
    void setProfile(Profile::Ptr profile);

    /**
     * Selects the text in the profile name edit area.
     * When the dialog is being used to create a new profile,
     * this can be used to draw the user's attention to the profile name
     * and make it easy for them to change it.
     */
    void selectProfileName();
    const Profile::Ptr lookupProfile() const;

public Q_SLOTS:
    // reimplemented
    void accept() Q_DECL_OVERRIDE;
    // reimplemented
    void reject() Q_DECL_OVERRIDE;

    void apply();

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    // sets up the specified tab page if necessary
    void preparePage(int);

    // saves changes to profile
    void save();

    // general page
    void selectInitialDir();
    void selectIcon();

    void profileNameChanged(const QString &name);
    void initialDirChanged(const QString &dir);
    void startInSameDir(bool);
    void commandChanged(const QString &command);
    void tabTitleFormatChanged(const QString &format);
    void remoteTabTitleFormatChanged(const QString &format);

    void terminalColumnsEntryChanged(int);
    void terminalRowsEntryChanged(int);
    void showTerminalSizeHint(bool);
    void setDimWhenInactive(bool);
    void showEnvironmentEditor();
    void silenceSecondsChanged(int);

    // appearance page
    void setFontSize(double pointSize);
    void setFontInputValue(const QFont &);
    void showAllFontsButtonWarning(bool enable);
    void setAntialiasText(bool enable);
    void setBoldIntense(bool enable);
    void useFontLineCharacters(bool enable);
    void showFontDialog();
    void newColorScheme();
    void editColorScheme();
    void saveColorScheme(const ColorScheme &scheme, bool isNewScheme);
    void removeColorScheme();
    void gotNewColorSchemes(const KNS3::Entry::List &changedEntries);

    /**
     * Deletes the selected colorscheme from the user's home dir location
     * so that the original one from the system-wide location can be used
     * instead
     */
    void resetColorScheme();

    void colorSchemeSelected();
    void previewColorScheme(const QModelIndex &index);
    void fontSelected(const QFont &);
    void toggleMouseWheelZoom(bool enable);

    // scrolling page
    void historyModeChanged(Enum::HistoryModeEnum mode);

    void historySizeChanged(int);

    void hideScrollBar();
    void showScrollBarLeft();
    void showScrollBarRight();

    void scrollFullPage();
    void scrollHalfPage();

    // keyboard page
    void editKeyBinding();
    void newKeyBinding();
    void keyBindingSelected();
    void removeKeyBinding();
    void resetKeyBindings();

    // mouse page
    void toggleUnderlineFiles(bool enable);
    void toggleUnderlineLinks(bool);
    void toggleOpenLinksByDirectClick(bool);
    void toggleCtrlRequiredForDrag(bool);
    void toggleDropUrlsAsText(bool);
    void toggleCopyTextToClipboard(bool);
    void toggleCopyTextAsHTML(bool);
    void toggleTrimLeadingSpacesInSelectedText(bool);
    void toggleTrimTrailingSpacesInSelectedText(bool);
    void pasteFromX11Selection();
    void pasteFromClipboard();
    void toggleAlternateScrolling(bool enable);

    void TripleClickModeChanged(int);
    void wordCharactersChanged(const QString &);

    // advanced page
    void toggleBlinkingText(bool);
    void toggleFlowControl(bool);
    void togglebidiRendering(bool);
    void lineSpacingChanged(int);
    void toggleBlinkingCursor(bool);
    void updateUrlHintsModifier(bool);
    void toggleReverseUrlHints(bool);

    void setCursorShape(int);
    void autoCursorColor();
    void customCursorColor();
    void customCursorColorChanged(const QColor &);
    void setDefaultCodec(QTextCodec *);

    // apply the first previewed changes stored up by delayedPreview()
    void delayedPreviewActivate();

private:
    Q_DISABLE_COPY(EditProfileDialog)

    // initialize various pages of the dialog
    void setupGeneralPage(const Profile::Ptr profile);
    void setupTabsPage(const Profile::Ptr profile);
    void setupAppearancePage(const Profile::Ptr profile);
    void setupKeyboardPage(const Profile::Ptr profile);
    void setupScrollingPage(const Profile::Ptr profile);
    void setupAdvancedPage(const Profile::Ptr profile);
    void setupMousePage(const Profile::Ptr info);

    // Returns the name of the colorScheme used in the current profile
    const QString currentColorSchemeName() const;

    // select @p selectedColorSchemeName after the changes are saved
    // in the colorScheme editor
    void updateColorSchemeList(const QString &selectedColorSchemeName = QString());

    void updateColorSchemeButtons();

    // Convenience method
    KeyboardTranslatorManager *_keyManager = KeyboardTranslatorManager::instance();

    // Updates the key bindings list widget on the Keyboard tab and selects
    // @p selectKeyBindingsName
    void updateKeyBindingsList(const QString &selectKeyBindingsName = QString());
    void updateKeyBindingsButtons();
    void showKeyBindingEditor(bool isNewTranslator);

    void showColorSchemeEditor(bool isNewScheme);
    void closeColorSchemeEditor();

    void preview(int property, const QVariant &value);
    void delayedPreview(int property, const QVariant &value);
    void unpreview(int property);
    void unpreviewAll();
    void enableIfNonEmptySelection(QWidget *widget, QItemSelectionModel *selectionModel);

    void updateCaption(const Profile::Ptr profile);
    void updateTransparencyWarning();

    // Update _tempProfile in a way of respecting the apply button.
    // When used with some previewed property, this method should
    // always come after the preview operation.
    void updateTempProfileProperty(Profile::Property, const QVariant &value);

    // helper method for creating an empty & hidden profile and assigning
    // it to _tempProfile.
    void createTempProfile();

    // Enable or disable apply button, used only within
    // updateTempProfileProperty().
    void updateButtonApply();

    static QString groupProfileNames(const ProfileGroup::Ptr group, int maxLength = -1);

    struct RadioOption {
        QAbstractButton *button;
        int value;
        const char *slot;
    };
    void setupRadio(const QVector<RadioOption>& possibilities, int actual);
    struct BooleanOption {
        QAbstractButton *button;
        Profile::Property property;
        const char *slot;
    };
    void setupCheckBoxes(const QVector<BooleanOption>& options, const Profile::Ptr profile);

    // returns false if:
    // - the profile name is empty
    // - the name matches the name of an already existing profile
    // - the existing profile config file is read-only
    // otherwise returns true.
    bool isValidProfileName();

    Ui::EditProfileDialog *_ui;
    Profile::Ptr _tempProfile;
    Profile::Ptr _profile;

    // keeps track of pages which need to be updated to match the current
    // profile.  all elements in this vector are set to true when the
    // profile is changed and individual elements are set to false
    // after an update by a call to ensurePageLoaded()
    QVector<bool> _pageNeedsUpdate;
    QHash<int, QVariant> _previewedProperties;

    QHash<int, QVariant> _delayedPreviewProperties;
    QTimer *_delayedPreviewTimer;

    ColorSchemeEditor *_colorDialog;
    QDialogButtonBox *mButtonBox;
};

/**
 * A delegate which can display and edit color schemes in a view.
 */
class ColorSchemeViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    explicit ColorSchemeViewDelegate(QObject *parent = nullptr);

    // reimplemented
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};
}

#endif // EDITPROFILEDIALOG_H
