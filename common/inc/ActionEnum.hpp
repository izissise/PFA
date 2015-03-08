#ifndef _ACTIONENUM_H_
#define _ACTIONENUM_H_

enum class	Action
{
  Unknown = -1,
    Forward = 0,
    Back,
    Right,
    Left,
    Use,
    MoveUp,
    MoveDown,
    ToggleConsole,
    ToggleQuickMenu,
    Chat,
    Last
    };


#endif /* _ACTIONENUM_H_ */
